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

#include <netcdf.h>
#include <memory>
#include <netcdf4_cxx/attribute.hpp>
#include <netcdf4_cxx/cf.hpp>
#include <netcdf4_cxx/dataset.hpp>
#include <netcdf4_cxx/scale_missing.hpp>

namespace netcdf {

constexpr ScaleMissing::ScaleMissing() noexcept
    : has_scale_offset_(false),
      has_valid_range_(false),
      has_valid_min_(false),
      has_valid_max_(false),
      has_fill_value_(false),
      has_missing_value_(false),
      valid_min_(NC_MIN_DOUBLE),
      valid_max_(NC_MAX_DOUBLE),
      scale_(1),
      offset_(0),
      fill_value_(NC_FILL_DOUBLE),
      missing_value_(NC_FILL_DOUBLE) {}

// ___________________________________________________________________________//

ScaleMissing::ScaleMissing(const DataSet& dataset)
    : valid_min_(NC_MIN_DOUBLE), valid_max_(NC_MAX_DOUBLE) {
  auto attribute = dataset.FindAttribute(CF::SCALE_FACTOR);
  scale_ = attribute != nullptr ? attribute->ReadScalar<double>() : 1;

  attribute = dataset.FindAttribute(CF::ADD_OFFSET);
  offset_ = attribute != nullptr ? attribute->ReadScalar<double>() : 0;

  has_scale_offset_ = scale_ != 1 || offset_ != 0;

  attribute = dataset.FindAttribute(CF::VALID_RANGE);
  has_valid_range_ = attribute != nullptr;
  if (has_valid_range_) {
    std::vector<double> values;
    attribute->Read(values);
    valid_min_ = values.at(0);
    valid_max_ = values.at(1);
  }

  attribute = dataset.FindAttribute(CF::VALID_MIN);
  has_valid_min_ = attribute != nullptr;
  if (has_valid_min_) {
    valid_min_ = attribute->ReadScalar<double>();
  }

  attribute = dataset.FindAttribute(CF::VALID_MAX);
  has_valid_max_ = attribute != nullptr;
  if (has_valid_max_) {
    valid_max_ = attribute->ReadScalar<double>();
  }

  attribute = dataset.FindAttribute(CF::FILL_VALUE);
  has_fill_value_ = attribute != nullptr;
  if (has_fill_value_) {
    fill_value_ = attribute->ReadScalar<double>();
  }

  attribute = dataset.FindAttribute(CF::MISSING_VALUE);
  has_missing_value_ = attribute != nullptr;
  if (has_missing_value_) {
    missing_value_ = attribute->ReadScalar<double>();
  }
}

}  // namespace netcdf
