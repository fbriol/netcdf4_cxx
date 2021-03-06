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
#include <netcdf4_cxx/object.hpp>

namespace netcdf {

/**
 * Abstract base class, from which Attribute, DataSet and Dimension inherit.
 * It provides the services that are common to both Attribute, DataSet and
 * Dimension.
 */
class AbstractDataSet : public Object {
 protected:
  int id_;  //!< DataSet ID

 public:
  /**
   * Default constructor
   */
  constexpr AbstractDataSet() noexcept : Object(), id_(NC_GLOBAL) {}

  /**
   * Create a global data set
   *
   * @param object NetCDF Object
   */
  explicit AbstractDataSet(const Object& object) noexcept : Object(object),
                                                            id_(NC_GLOBAL) {}

  /**
   * Create a specific data set
   *
   * @param object NetCDF Object
   * @param id Data set ID
   */
  AbstractDataSet(const Object& object, const int id) noexcept : Object(object),
                                                                 id_(id) {}

  /**
   * Destructive
   */
  virtual ~AbstractDataSet() noexcept {}

  /**
   * Get the data set ID
   * @return the data set ID
   */
  int id() const noexcept { return id_; }

  /**
   * Is global dataset?
   *
   * @return true if global dataset
   */
  bool IsGlobal() const noexcept { return id_ == NC_GLOBAL; }

  /**
   * Test if two NetCDF data set are different
   *
   * @param rhs Other data set to compare
   * @return true if the two instances are different
   */
  bool operator!=(const Object& rhs) const noexcept override {
    const AbstractDataSet* inherited =
        dynamic_cast<const AbstractDataSet*>(&rhs);
    if (inherited != nullptr) {
      return Object::operator!=(rhs) or id_ != inherited->id_;
    }
    return true;
  }

  /**
   * Test if two NetCDF data set are equal
   *
   * @param rhs Other data set to compare
   * @return true if the two instances are equal
   */
  bool operator==(const Object& rhs) const noexcept override {
    const AbstractDataSet* inherited =
        dynamic_cast<const AbstractDataSet*>(&rhs);
    if (inherited != nullptr) {
      return Object::operator==(rhs) and id_ == inherited->id_;
    }
    return false;
  }
};

}  // namespace netcdf
