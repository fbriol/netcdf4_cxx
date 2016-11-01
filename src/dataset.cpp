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


#include <boost/algorithm/string/case_conv.hpp>
#include <netcdf/dataset.hpp>
#include <stdexcept>

namespace netcdf {

std::list<Attribute> DataSet::GetAttributes() const {
  int natts;

  if (id_ == NC_GLOBAL) {
    Check(nc_inq_natts(nc_id_, &natts));
  } else {
    Check(nc_inq_varnatts(nc_id_, id_, &natts));
  }

  std::list<Attribute> result;
  char name[NC_MAX_NAME + 1];

  for (int ix = 0; ix < natts; ++ix) {
    Check(nc_inq_attname(nc_id_, id_, ix, name));
    result.push_back(Attribute(*this, name));
  }
  return result;
}

std::shared_ptr<Attribute> DataSet::FindAttribute(const std::string& name,
     bool ignore_case) const {
  // Search an attribute by its name, if ignore case
  if (ignore_case) {
    auto lower_name = boost::to_lower_copy<std::string>(name);
    auto attributes = GetAttributes();
    auto it = std::find_if(attributes.begin(), attributes.end(),
                           [&] (const Attribute& attribute) {
                             return boost::to_lower_copy<std::string>(
                                 attribute.name()) == lower_name;
                           });
    if (it != attributes.end())
      return std::make_shared<Attribute>(*it);
  } else {
    // Query the C-API to get the attribute by its name
    int id;
    if (nc_inq_attid(nc_id_, id_, name.c_str(), &id) == NC_NOERR)
      return std::make_shared<Attribute>(*this, name);
  }
  return std::shared_ptr<Attribute>(nullptr);
}

}  // namespace netcdf
