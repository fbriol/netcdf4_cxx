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


#include <netcdf/dimension.hpp>
#include <netcdf/group.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

namespace netcdf {

bool Dimension::IsUnlimited() const {
  int num_unlimited_dimensions;

  Check(nc_inq_unlimdims(nc_id_, &num_unlimited_dimensions,
                            NULL));

  if (num_unlimited_dimensions != 0) {
    std::vector<int> unlimited_dimensions(num_unlimited_dimensions);
    Check(
        nc_inq_unlimdims(nc_id_, &num_unlimited_dimensions,
                         &unlimited_dimensions[0]));
    return std::find(unlimited_dimensions.begin(), unlimited_dimensions.end(),
                     id_) != unlimited_dimensions.end();
  }
  return false;
}

std::string Dimension::GetLongName() const {
  return Group(*this).GetLongName() + "/" + GetShortName();
}

void Dimension::Copy(const Object& object) const {
  Group(object).AddDimension(GetShortName(),
                             IsUnlimited() ? NC_UNLIMITED: GetLength());
}

}  // namespace netcdf
