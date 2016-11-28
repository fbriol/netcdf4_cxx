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

#pragma once

#include <netcdf.h>
#include <stdexcept>

namespace netcdf {

/**
 * Base class of errors thrown by this library
 */
class Error : public std::runtime_error {
  /**
   * Constructor
   *
   * @param status NetCDF status code
   */
 public:
  Error(const int status) : std::runtime_error(nc_strerror(status)) {}
};

/**
 * Check the return code of a netCDF-C function and if necessary throws an
 * exception
 *
 * @param status Code to check
 */
inline void Check(const int status) {
  if (status != NC_NOERR) throw Error(status);
}

}  // namespace netcdf
