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

#include <netcdf4_cxx/abstract_dataset.hpp>
#include <netcdf4_cxx/group.hpp>
#include <regex>
#include <stdexcept>

namespace netcdf {

Variable Group::AddVariable(const std::string& name, const type::Generic& type,
                            const std::vector<Dimension>& dimensions) const {
  std::vector<int> dimids;
  int var_id;

  for (auto& item : dimensions) {
    dimids.push_back(item.id());
  }
  Check(nc_def_var(nc_id_, name.c_str(), type.id(),
                   static_cast<int>(dimids.size()),
                   dimids.size() ? &dimids[0] : nullptr, &var_id));
  return Variable(*this, var_id);
}

std::vector<Dimension> Group::GetDimensions() const {
  std::vector<int> dimension_id(NC_MAX_DIMS);
  std::vector<Dimension> result;
  int num_dimensions;

  Check(nc_inq_dimids(nc_id_, &num_dimensions, &dimension_id[0], 0));
  if (num_dimensions == 0) return result;

  dimension_id.resize(num_dimensions);
  for (int& dim_id : dimension_id) {
    result.push_back(Dimension(*this, dim_id));
  }

  return result;
}

std::shared_ptr<Dimension> Group::FindDimension(const std::string& name) const {
  std::shared_ptr<Dimension> result(nullptr);
  Group item = *this;
  while (true) {
    result = item.FindDimensionLocal(name);
    if (result != nullptr or item.IsRoot()) break;
    item = item.GetParentGroup();
  }
  return result;
}

std::list<Group> Group::GetGroups() const {
  std::list<Group> result;
  int num_groups;

  Check(nc_inq_grps(nc_id_, &num_groups, nullptr));
  if (num_groups == 0) return result;

  std::vector<int> group_ident(num_groups);

  Check(nc_inq_grps(nc_id_, nullptr, &group_ident[0]));

  for (auto& nc_id : group_ident) {
    result.push_back(Group(nc_id));
  }

  return result;
}

std::shared_ptr<Group> Group::GetCommonParent(const Group& other) const {
  for (auto& my : *this) {
    if (my.IsRoot() or my == *this) continue;
    for (auto& its : other) {
      if (its.IsRoot() or its == other) continue;
      if (its.nc_id() == my.nc_id()) return std::make_shared<Group>(my);
    }
  }
  return std::shared_ptr<Group>(nullptr);
}

std::string Group::GetLongName() const {
  size_t length;

  Check(nc_inq_grpname_len(nc_id_, &length));
  std::string result(length, 0);
  Check(nc_inq_grpname_full(nc_id_, &length, &result[0]));

  return result;
}

std::list<Variable> Group::GetVariables() const {
  std::list<Variable> result;
  int num_variables;

  Check(nc_inq_nvars(nc_id_, &num_variables));
  if (num_variables == 0) return result;

  std::vector<int> variable_ident(num_variables);

  Check(nc_inq_varids(nc_id_, &num_variables, &variable_ident[0]));

  for (auto& var_id : variable_ident) {
    result.push_back(Variable(*this, var_id));
  }

  return result;
}

bool Group::IsRoot() const {
  int parent;
  int status = nc_inq_grp_parent(nc_id_, &parent);

  if (status != NC_NOERR && status != NC_ENOGRP)
    throw std::runtime_error(nc_strerror(status));

  return status == NC_ENOGRP;
}

void Group::Copy(const Group& target,
                 const std::list<std::string>& variables) const {
  for (auto& item : GetDataTypes()) {
    item.Copy(target);
  }

  for (auto& item : GetDimensions()) {
    item.Copy(target);
  }

  for (auto& item : GetAttributes()) {
    item.Copy(target);
  }

  for (auto& item : GetVariables()) {
    if (std::find(variables.begin(), variables.end(), item.GetLongName()) ==
        variables.end())
      item.Copy(target);
  }

  for (auto& item : GetGroups()) {
    item.Copy(Group(target, item.GetShortName()), variables);
  }
}

std::list<type::Generic> Group::GetDataTypesLocal() const {
  int ntypes;

  Check(nc_inq_typeids(nc_id_, &ntypes, nullptr));
  std::vector<int> user_types(ntypes);
  std::list<type::Generic> result;

  Check(nc_inq_typeids(nc_id_, nullptr, &user_types[0]));
  for (auto& item : user_types) {
    result.push_back(type::Generic(*this, item));
  }
  return result;
}

std::list<type::Generic> Group::GetDataTypes() const {
  auto result = GetDataTypesLocal();
  for (auto& item : GetGroups()) {
    auto types = item.GetDataTypes();
    result.insert(result.begin(), types.begin(), types.end());
  }
  return result;
}

std::shared_ptr<Variable> Group::FindVariableOrInParent(
    const std::string& name) const {
  std::shared_ptr<Variable> result(nullptr);
  Group item = *this;
  while (true) {
    result = item.FindVariable(name);
    if (result != nullptr or item.IsRoot()) break;
    item = item.GetParentGroup();
  }
  return result;
}

std::list<Group> Group::Walk() const {
  std::list<Group> result, groups = GetGroups();
  for (auto& item : groups) {
    std::list<Group> childs = item.Walk();
    result.insert(result.end(), childs.begin(), childs.end());
  }
  result.insert(result.begin(), groups.begin(), groups.end());
  return result;
}

std::shared_ptr<type::Generic> Group::FindDataType(
    const std::string& name) const {
  std::shared_ptr<type::Generic> result(nullptr);
  Group item = *this;
  while (true) {
    for (auto& type : item.GetDataTypesLocal()) {
      if (type.GetName() == name) {
        result = std::make_shared<type::Generic>(type);
        break;
      }
    }
    if (result != nullptr or item.IsRoot()) break;
    item = item.GetParentGroup();
  }
  return result;
}

std::pair<std::list<std::string>, std::string> Group::SplitGroupsAndVariable(
    const std::string& path) {
  std::pair<std::list<std::string>, std::string> result;
  std::regex re("/");
  std::sregex_token_iterator first{path.begin(), path.end(), re, -1}, last;

  result.first = std::list<std::string>{first, last};
  if (path.back() != '/' && result.first.size()) {
    result.second = result.first.back();
    result.first.pop_back();
  }

  return result;
}

}  // namespace netcdf
