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

namespace netcdf {

class Iterator;
class Group;

/**
 * NetCDF Object is a base class.
 */
class Object {
 protected:
  int nc_id_;  ///<! NetCDF ID

  /**
   * Default constructor
   */
  Object()
      : nc_id_(0) {
  }

  /**
   * Create a copy of an existing object giving the Object ID
   *
   * @param nc_id NetCDF ID
   */
  explicit Object(const int nc_id)
      : nc_id_(nc_id) {
  }

 public:
  /**
   * Get the Group
   *
   * @return The Group handled by this instance
   */
  Group GetGroup() const;

  /**
   * Get its parent Group, throwing an std::logic_error exception if its the
   * root group.
   *
   * @return parent group
   * @throw std::logic_error if it's the root group
   */
  Group GetParentGroup() const;

  /**
   * Get the NetCDF ID handled
   *
   * @return the NetCDF ID handled
   */
  int nc_id() const {
    return nc_id_;
  }

  /**
   * Test if two NetCDF Object are different
   *
   * @param rhs Other object to compare
   * @return true if the two instances are different
   */
  bool operator != (const Object& rhs) const {
    return nc_id_ != rhs.nc_id_;
  }

  /**
   * Test if two NetCDF Object are equal
   *
   * @param rhs Other object to compare
   * @return true if the two instances are equal
   */
  bool operator == (const Object& rhs) const {
    return nc_id_ == rhs.nc_id_;
  }
};

}  // namespace netcdf
