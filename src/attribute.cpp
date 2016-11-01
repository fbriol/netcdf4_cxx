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


#include <netcdf4_cxx/attribute.hpp>
#include <stdlib.h>

namespace netcdf {

class CStringArray : public std::vector<char*> {
 public:
  explicit CStringArray(const size_t length)
      : std::vector<char *>(length, 0) {
  }

  ~CStringArray() {
    for (auto& item : *this) {
      // Allocated by NetCDF C API
      free(item);
    }
  }
};

std::vector<std::string> Attribute::ReadString() const {
  CStringArray buffer(GetLength());
  std::vector<std::string> result;

  Check(nc_get_att_string(nc_id_, id_, name_.c_str(), &buffer[0]));
  for (auto& item : buffer) {
    result.push_back(item);
  }
  return result;
}

}  // namespace netcdf
