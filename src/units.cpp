/* This file is part of NetCDF4_CXX library.

 NetCDF4_CXX is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NetCDF4_CXX is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with NetCDF4_CXX.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <udunits2.h>
#include <cerrno>
#include <netcdf4_cxx/units.hpp>

namespace netcdf {
namespace units {

using UtUnitPtr = std::unique_ptr<ut_unit, decltype(&ut_free)>;
using UtConverterPtr = std::unique_ptr<cv_converter, decltype(&cv_free)>;

static void HandleParseStatus(const ut_unit* const unit,
                              const std::string& str) {
  if (unit == nullptr) {
    ut_status status = ut_get_status();
    if (status == UT_BAD_ARG) throw Error("empty units attribute string");
    if (status == UT_SYNTAX)
      throw Error("'" + str + "' contained a syntax error");
    if (status == UT_UNKNOWN)
      throw Error("'" + str + "' string contained an unknown identifier");
    throw Error("Not handled exception: " + std::to_string(status));
  }
}

static void HandleConverterStatus(const cv_converter* const converter,
                                  const std::string& from,
                                  const std::string& to) {
  if (converter == nullptr) {
    ut_status status = ut_get_status();
    if (status == UT_BAD_ARG) throw Error("one of units is null");
    if (status == UT_NOT_SAME_SYSTEM)
      throw Error("the units '" + from + "' and '" + to +
                  "' don't belong to the same unit-system");
    if (status == UT_MEANINGLESS)
      throw Error("the units '" + from + "' and '" + to +
                  "' belong to the same unit-system but conversion " +
                  "between them is meaningless");
    throw Error("Not handled exception: " + std::to_string(status));
  }
}

void Parser::Initialize(const char* const path) {
  if (!system_) {
    ut_set_error_message_handler(&ut_ignore);
    system_ = std::shared_ptr<ut_system>(ut_read_xml(path), ut_free_system);
    if (!system_) {
      ut_status status = ut_get_status();
      std::error_code error_code(errno, std::generic_category());
      throw SystemError(
          error_code,
          std::string("Couldn't initialize unit-system from database '") +
              ut_get_path_xml(path, &status) + "'");
    }
  }
}

Converter Parser::Parse(const std::string& from, const std::string& to) const {
  if (from == to) {
    return Converter();
  } else {
    UtUnitPtr ut_from(ut_parse(system_.get(), from.c_str(), UT_UTF8), ut_free);
    HandleParseStatus(ut_from.get(), from);

    UtUnitPtr ut_to(ut_parse(system_.get(), to.c_str(), UT_UTF8), ut_free);
    HandleParseStatus(ut_from.get(), to);

    UtConverterPtr converter(ut_get_converter(ut_from.get(), ut_to.get()),
                             cv_free);
    HandleConverterStatus(converter.get(), from, to);

    double offset = cv_convert_double(converter.get(), 0);
    double scale = cv_convert_double(converter.get(), 1) - offset;

    return Converter(offset, scale);
  }
}

bool Parser::AreConvertible(const std::string& unit1,
                            const std::string& unit2) const {
  UtUnitPtr ut_unit1(ut_parse(system_.get(), unit1.c_str(), UT_UTF8), ut_free);
  HandleParseStatus(ut_unit1.get(), unit1);

  UtUnitPtr ut_unit2(ut_parse(system_.get(), unit2.c_str(), UT_UTF8), ut_free);
  HandleParseStatus(ut_unit2.get(), unit2);

  return ut_are_convertible(ut_unit1.get(), ut_unit2.get()) != 0;
}

}  // namespace units
}  // namespace netcdf
