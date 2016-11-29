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

#include <stdlib.h>
#include <netcdf4_cxx/attribute.hpp>

namespace netcdf {

class CStringArray : public std::vector<char*> {
 public:
  explicit CStringArray(const size_t length) : std::vector<char*>(length, 0) {}

  ~CStringArray() {
    for (auto& item : *this) {
      // Allocated by NetCDF C API
      free(item);
    }
  }
};

#define __NETCDF4CXX_READ_ATT(type, sufix)                             \
  template <>                                                          \
  std::vector<type> Attribute::Read() const {                          \
    std::vector<type> values(GetLength());                             \
    Check(nc_get_att_##sufix(nc_id_, id_, name_.c_str(), &values[0])); \
    return values;                                                     \
  }

__NETCDF4CXX_READ_ATT(signed char, schar)
__NETCDF4CXX_READ_ATT(unsigned char, uchar)
__NETCDF4CXX_READ_ATT(short, short)
__NETCDF4CXX_READ_ATT(unsigned short, ushort)
__NETCDF4CXX_READ_ATT(int, int)
__NETCDF4CXX_READ_ATT(unsigned int, uint)
__NETCDF4CXX_READ_ATT(long long, longlong)
__NETCDF4CXX_READ_ATT(unsigned long long, ulonglong)
__NETCDF4CXX_READ_ATT(float, float)
__NETCDF4CXX_READ_ATT(double, double)

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
