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
#include <netcdf/abstract_dataset.hpp>
#include <netcdf/netcdf.hpp>
#include <stddef.h>
#include <string>

namespace netcdf {

/**
 * A Dimension is used to define the array shape of a Variable. It may be
 * shared among Variables, which provides a simple yet powerful way of
 * associating Variables. When a Dimension is shared, it has a unique name
 * within its Group. It may have a coordinate Variable, which gives each index
 * a coordinate value. A private Dimension cannot have a coordinate Variable,
 * so use shared dimensions with coordinates when possible. The Dimension
 * length must be > 0, except for an unlimited dimension which may have
 * length = 0, and a variable length Dimension which has length = -1.
 */
class Dimension : public AbstractDataSet {
 public:
  /**
   * Create a copy of an existing Dimension using its ID
   *
   * @param object NetCDF object
   * @param dim_id Dimension ID
   */
  Dimension(const Object& object, const int dim_id)
      : AbstractDataSet(object, dim_id) {
  }

  /**
   * Copy the dimension from the netCDF object to another
   *
   * @param object target object
   */
  void Copy(const Object& object) const;

  /**
   * Get the length of the Dimension.
   *
   * @return the length
   */
  inline size_t GetLength() const {
    size_t result;
    Check(nc_inq_dimlen(nc_id_, id_, &result));
    return result;
  }

  /**
   * Rename the dimension
   *
   * @param name the new dimension name
   */
  void Rename(const std::string& name) {
    Check(nc_rename_dim(nc_id_, id_, name.c_str()));
  }

  /**
   * If unlimited, then the length can increase; otherwise it is immutable.
   *
   * @return if its an "unlimited" Dimension
   */
  bool IsUnlimited() const;

  /**
   * Get the short name of this Dimension.
   *
   * @return the short name
   */
  inline std::string GetShortName() const {
    char result[NC_MAX_NAME + 1];
    Check(nc_inq_dimname(nc_id_, id_, result));
    return result;
  }

  /**
   * Get the long name of this Dimension.
   *
   * @return the long name
   */
  std::string GetLongName() const;
};

}  // namespace netcdf
