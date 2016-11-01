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

#include <netcdf4_cxx/group.hpp>
#include <netcdf4_cxx/object.hpp>
#include <netcdf.h>
#include <stdexcept>

namespace netcdf {

Group Object::GetGroup() const {
  return Group(*this);
}

Group Object::GetParentGroup() const {
  int parent;
  int status = nc_inq_grp_parent(nc_id_, &parent);

  if (status == NC_ENOGRP)
    throw std::logic_error(nc_strerror(NC_ENOGRP));
  else if (status != NC_NOERR)
    throw Error(status);

  return Group(Object(parent));
}

}
