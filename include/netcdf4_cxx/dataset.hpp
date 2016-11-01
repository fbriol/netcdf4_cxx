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

#include <netcdf4_cxx/attribute.hpp>
#include <netcdf4_cxx/netcdf.hpp>
#include <netcdf.h>
#include <iostream>
#include <list>
#include <memory>
#include <string>

namespace netcdf {

/**
 * Abstract base class, from which Group and Variable inherit.
 * It provides the services that are common to both Group and
 * Variable.
 */
class DataSet : public AbstractDataSet {
 public:
  /**
   * Default constructor
   */
  DataSet()
      : AbstractDataSet() {
  }

  /**
   * Create a new data set from an existing NetCDF Object
   *
   * @param object NetCDF Object
   */
  explicit DataSet(const Object& object)
      : AbstractDataSet(object) {
  }

  /**
   * Create a new data set from an existing NetCDF Object and NetCDF
   * Variable
   *
   * @param object NetCDF Object
   * @param var_id Variable ID
   */
  DataSet(const Object& object, const int var_id)
      : AbstractDataSet(object, var_id) {
  }

  /**
   * Get the set of attributes contained in this container.
   *
   * @return list of attributes
   */
  std::list<Attribute> GetAttributes() const;

  /**
   * Find an Attribute by its name
   *
   * @param name attribute name
   * @param ignore_case ignore case if true
   * @return the attribute read or NULL if not found
   */
  std::shared_ptr<Attribute> FindAttribute(const std::string& name,
                                           bool ignore_case = false) const;

  /**
   * Test if attribute exists
   *
   * @param name attribute name
   * @param ignore_case ignore_case if true
   * @return true if the attribute exists otherwise false.
   */
  bool AttributeExists(const std::string& name,
                       bool ignore_case = false) const {
    return FindAttribute(name, ignore_case) != nullptr;
  }

  /**
   * Add new Attribute; replace old if has same name.
   *
   * @param name Attribute name
   */
  Attribute AddAttribute(const std::string& name) const {
    return Attribute(*this, name);
  }

  /**
   * Remove an Attribute using its (short) name, in this group only
   *
   * @param name attribute name to remove
   */
  void RemoveAttribute(const std::string& name) const {
    Check(nc_del_att(nc_id_, id_, name.c_str()));
  }
};

}  // namespace netcdf
