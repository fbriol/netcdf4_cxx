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

#include <netcdf/dimension.hpp>
#include <netcdf/netcdf.hpp>
#include <netcdf/object.hpp>
#include <netcdf/type.hpp>
#include <netcdf/variable.hpp>
#include <netcdf.h>
#include <stddef.h>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <string>
#include <vector>

namespace netcdf {

/**
 * Iterate over Group
 */
template<typename T>
class GroupIteratorType : public std::iterator<std::forward_iterator_tag, T> {
 private:
  T* group_;

 public:
  /**
   * Default constructor
   *
   * @param group Group to iterate
   */
  explicit GroupIteratorType(T* group) {
    if (group != nullptr) {
      group_ = new T(*group);
    } else {
      group_ = nullptr;
    }
  }

  /**
   * Destructive
   */
  ~GroupIteratorType() {
    delete group_;
  }

  /**
   * Test GroupIteratorType if the iteration is over
   *
   * @return true if the iteration is over
   */
  operator bool() const {
    return group_ ? group_->IsRoot() : false;
  }

  /**
   * Test whether two iterators are equal
   *
   * @param other Iterator to compare
   * @return true if the two iterators are the same
   */
  bool operator==(const GroupIteratorType& other) const {
    return (group_ == other.group_);
  }

  /**
   * Test whether two iterators are different
   *
   * @param other Iterator to compare
   * @return true if the two iterators are different
   */
  bool operator!=(const GroupIteratorType& iterator) const {
    return (group_ != iterator.group_);
  }

  /**
   * Move forward to the next group
   *
   * @return a reference to this instance
   */
  GroupIteratorType& operator++() {
    if (group_->IsRoot()) {
      delete group_;
      group_ = nullptr;
    } else {
      *group_ = group_->GetParentGroup();
    }
    return *this;
  }

  /**
   * Dereferences pointer to the handled instance
   *
   * @return a reference to the handled Group by this instance
   */
  T& operator*() {
    return *group_;
  }

  /**
   * Dereferences pointer to the handled instance
   *
   * @return a reference to the handled Group by this instance
   */
  const T& operator*() const {
    return *group_;
  }

  /**
   * Pointer to member instance
   *
   * @return a reference to the handled Group by this instance
   */
  T* operator->() {
    return group_;
  }
};

/**
 * A Group is a logical collection of Variables. The Groups in a Dataset form a
 * hierarchical tree, like directories on a disk. A Group has a name and
 * optionally a set of Attributes. There is always at least one Group in a
 * dataset, the root Group, whose name is the empty string.
 */
class Group : public DataSet {
 private:
  /**
   * Create a new group from a Group ID
   *
   * @param group_id Group ID
   */
  explicit Group(const int group_id) {
    nc_id_ = group_id;
  }

 protected:
  /**
   * Create an empty Group
   */
  Group()
      : DataSet() {
  }

 public:
  /**
   * Default constructor
   *
   * @param object NetCDF Object
   */
  explicit Group(const Object& object)
      : Group(object.nc_id()) {
  }

  /**
   * Create a nested group relative to the given Object
   *
   * @param object Parent group
   * @param name Group name
   */
  Group(const Object& object, const std::string& name) {
    Check(
        nc_def_grp(object.nc_id(), const_cast<char*>(name.c_str()), &nc_id_));
  }

  /**
   * Get the short name of this Dimension.
   *
   * @return the name
   */
  std::string GetShortName() const {
    char result[NC_MAX_NAME + 1];
    Check(nc_inq_grpname(nc_id_, result));
    return result;
  }

  /**
   * Get the long name of this Dimension.
   *
   * @return the name
   */
  std::string GetLongName() const;

  /**
   * Is this the root group?
   *
   * @return true if root group
   */
  bool IsRoot() const;

  /**
   * Add a dimension
   *
   * @param name dimension name
   * @param size dimension size
   * @return the added dimension
   */
  Dimension AddDimension(const std::string& name, const size_t size) const {
    int dim_id;
    Check(nc_def_dim(nc_id_, name.c_str(), size, &dim_id));
    return Dimension(*this, dim_id);
  }

  /**
   * Add an unlimited dimension
   *
   * @param name dimension name
   */
  Dimension AddUnlimitedDimension(const std::string& name) const {
    return AddDimension(name, NC_UNLIMITED);
  }

  /**
   * Add a variable
   *
   * @param name Variable name
   * @param type Variable type
   * @param dimensions Variable dimensions
   * @return the variable created
   */
  Variable AddVariable(const std::string& name, const type::Generic& type,
     const std::vector<Dimension>& dimensions = std::vector<Dimension>()) const;

  /**
   * Add a nested group
   *
   * @param name Group name
   * @return the group added
   */
  Group AddGroup(const std::string& name) const {
    int id;
    Check(nc_def_grp(nc_id_, name.c_str(), &id));
    return Group(id);
  }

  /**
   * Get the Dimensions contained directly in this group.
   *
   * @return list of dimensions
   */
  std::vector<Dimension> GetDimensions() const;

  /**
   * Retrieve a Dimension using its (short) name, in this group only
   *
   * @param name dimension name
   * @return the dimension or null if not found
   */
  std::shared_ptr<Dimension> FindDimensionLocal(const std::string& name) const {
    int dim_id;
    if(nc_inq_dimid(nc_id_, name.c_str(), &dim_id) == NC_NOERR)
      return std::make_shared<Dimension>(*this, dim_id);
    return std::shared_ptr<Dimension>(nullptr);
  }

  /**
   * Retrieve a Dimension using its (short) name.
   * If it doesn't exist in this group, recursively look in parent groups.
   *
   * @param name dimension name
   * @return the dimension or null if not found
   */
  std::shared_ptr<Dimension> FindDimension(const std::string& name) const;

  /**
   * Get the Groups contained directly in this Group.
   *
   * @return list of groups
   */
  std::list<Group> GetGroups() const;

  /**
   * Get the common parent of this and the other group.
   *
   * @param group the other Group
   * @return common parent of this and the other Group or NULL if the
   *  common parent is the root Group
   */
  std::shared_ptr<Group> GetCommonParent(const Group& other) const;

  /**
   * Retrieve the Group with the specified (short) name.
   *
   * @param name Short name of the nested group you are looking for.
   * @return the Group, or NULL if not found
   */
  std::shared_ptr<Group> FindGroup(const std::string& name) const {
    for (auto& item: GetGroups()) {
      if (item.GetShortName() == name) {
        return std::make_shared<Group>(item);
      }
    }
    return std::shared_ptr<Group>(nullptr);
  }

  /**
   * Find the Variable with the specified (short) name in this group.
   *
   * @param name Short name of Variable within this group.
   * @return the Variable, or null if not found
   */
  std::shared_ptr<Variable> FindVariable(const std::string& name) const {
    int var_id;
    if (nc_inq_varid(nc_id_, name.c_str(), &var_id) == NC_NOERR)
        return std::make_shared<Variable>(*this, var_id);
    return std::shared_ptr<Variable>(nullptr);
  }

  /**
   * Find the Variable with the specified (short) name in this group or a parent group.
   *
   * @param name Short name of Variable within this group.
   * @return the Variable, or null if not found
   */
  std::shared_ptr<Variable> FindVariableOrInParent(const std::string& name) const;

  /**
   * Walk the group tree down-top
   *
   * @return list of groups
   */
  std::list<Group> Walk() const;

  /**
   * Get the root group
   *
   * @return the root group
   */
  Group GetRootGroup() const {
    Group result = *this;
    while(not result.IsRoot()) {
      result = result.GetParentGroup();
    }
    return result;
  }

  /**
   * Retrieve the Group with the specified (short) name
   *
   * @param name group name
   * @return the group
   */
  Group GetNamedGroup(const std::string& name) const {
    int nc_id = -1;  // CLANG -Wuninitialized
    nc_inq_grp_ncid(nc_id, name.c_str(), &nc_id);
    return Group(nc_id);
  }

  /**
   * Get the Variables contained directly in this group.
   *
   * @return the list of variables
   */
  std::list<Variable> GetVariables() const;

  /**
   * Copy the group from the netCDF data set to another
   *
   * @param target Target group
   * @param variables List of variables to be ignored in the source group
   */
  void Copy(const Group& target,
            const std::list<std::string>& variables) const;

  /**
   * Get the user data type defined in this group only
   *
   * @return user data type defined
   */
  std::list<type::Generic> GetDataTypesLocal() const;

  /**
   * Remove an Attribute
   *
   * @param attribute Attribute to remove
   * @return true if was found and removed
   */
  bool Remove(const Attribute& attribute) const {
    if (AttributeExists(attribute.name())) {
      RemoveAttribute(attribute.name());
      return true;
    }
    return false;
  }

  /**
   * Get the user data type defined in this Group and parent Group
   *
   * @return user data type defined
   */
  std::list<type::Generic> GetDataTypes() const;

  /**
   * Get the user data type using its (short) name, If it doesn't exist in this
   * Group, recursively look in parent groups.
   *
   * @return user data type defined or NULL if not found
   */
  std::shared_ptr<type::Generic> FindDataType(const std::string& name) const;

  /// Define an iterator over the parent Group
  typedef GroupIteratorType<Group> GroupIterator;

  /// Define an constant iterator over the parent Group
  typedef GroupIteratorType<Group> ConstGroupIterator;

  /**
   * Return an iterator to the beginning
   *
   * @return an iterator
   */
  GroupIterator begin() {
    return GroupIterator(this);
  }

  /**
   * Return an iterator to the end
   *
   * @return an iterator
   */
  GroupIterator end() {
    return GroupIterator(nullptr);
  }

  /**
   * Return a constant iterator to the beginning
   *
   * @return a constant iterator
   */
  ConstGroupIterator begin() const {
    return ConstGroupIterator(const_cast<Group*>(this));
  }

  /**
   * Return a constant iterator to the beginning
   *
   * @return a constant iterator
   */
  ConstGroupIterator end() const {
    return ConstGroupIterator(nullptr);
  }
};

}  // namespace netcdf
