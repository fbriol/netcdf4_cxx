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

#include <list>
#include <netcdf4_cxx/abstract_dataset.hpp>
#include <netcdf4_cxx/attribute.hpp>
#include <netcdf4_cxx/group.hpp>
#include <netcdf4_cxx/object.hpp>
#include <netcdf4_cxx/variable.hpp>

namespace netcdf {

std::vector<Dimension> Variable::GetDimensions() const {
  std::vector<int> dimension_ident(GetRank());
  std::vector<Dimension> result;

  Check(nc_inq_vardimid(nc_id_, id_, &dimension_ident[0]));

  for (auto& item : dimension_ident) {
    result.push_back(Dimension(*this, item));
  }
  return result;
}

std::string Variable::GetLongName() const {
  return Group(*this).GetLongName() + "/" + GetShortName();
}

bool Variable::IsCoordinate() const {
  std::vector<Dimension> dimensions = GetDimensions();
  type::Primitive data_type = GetDataType().GetPrimitive();

  if (data_type == type::Primitive::kCompound ||
      data_type == type::Primitive::kOpaque ||
      data_type == type::Primitive::kEnum ||
      data_type == type::Primitive::kVLen)
    return false;

  if (dimensions.size() == 1) {
    if (GetShortName() == dimensions.front().GetShortName()) return true;
  } else if (dimensions.size() == 2) {
    if (GetShortName() == dimensions.front().GetShortName() &&
        data_type == type::Primitive::kChar)
      return true;
  }
  return false;
}

#define __NETCDF4CXX_READ_VAR(type, sufix)                                 \
  template <>                                                              \
  std::vector<type> Variable::Read(const Hyperslab& hyperslab) const {     \
    if (hyperslab > GetShape())                                            \
      throw std::invalid_argument(                                         \
          "Hyperslab defined overlap the "                                 \
          "variable definition");                                          \
    std::vector<type> values(hyperslab.GetSize());                         \
    if (hyperslab.OnlyAdjacent())                                          \
      Check(nc_get_vara_##sufix(nc_id_, id_, &hyperslab.start()[0],        \
                                &hyperslab.GetSizeList()[0], &values[0])); \
    else                                                                   \
      Check(nc_get_vars_##sufix(nc_id_, id_, &hyperslab.start()[0],        \
                                &hyperslab.GetSizeList()[0],               \
                                &hyperslab.step()[0], &values[0]));        \
    return values;                                                         \
  }

__NETCDF4CXX_READ_VAR(signed char, schar)
__NETCDF4CXX_READ_VAR(unsigned char, uchar)
__NETCDF4CXX_READ_VAR(short, short)
__NETCDF4CXX_READ_VAR(unsigned short, ushort)
__NETCDF4CXX_READ_VAR(int, int)
__NETCDF4CXX_READ_VAR(unsigned int, uint)
__NETCDF4CXX_READ_VAR(long long, longlong)
__NETCDF4CXX_READ_VAR(unsigned long long, ulonglong)
__NETCDF4CXX_READ_VAR(float, float)
__NETCDF4CXX_READ_VAR(double, double)

#define __NETCDF4CXX_WRITE_VAR(type, sufix)                                  \
  void Variable::Write(const Hyperslab& hyperslab,                           \
                       const std::vector<type>& values) const {              \
    if (hyperslab.IsEmpty()) {                                               \
      if (IsUnlimited())                                                     \
        throw std::runtime_error(                                            \
            "You must specify a hyperslab for "                              \
            "unlimited variables");                                          \
      Check(nc_put_var_##sufix(nc_id_, id_, &values[0]));                    \
    } else {                                                                 \
      if (values.size() != hyperslab.GetSize())                              \
        throw std::invalid_argument(                                         \
            "data size does not match hyperslab "                            \
            "definition");                                                   \
      if (hyperslab.OnlyAdjacent())                                          \
        Check(nc_put_vara_##sufix(nc_id_, id_, &hyperslab.start()[0],        \
                                  &hyperslab.GetSizeList()[0], &values[0])); \
      else                                                                   \
        Check(nc_put_vars_##sufix(nc_id_, id_, &hyperslab.start()[0],        \
                                  &hyperslab.GetSizeList()[0],               \
                                  &hyperslab.step()[0], &values[0]));        \
    }                                                                        \
  }

__NETCDF4CXX_WRITE_VAR(signed char, schar)
__NETCDF4CXX_WRITE_VAR(unsigned char, uchar)
__NETCDF4CXX_WRITE_VAR(short, short)
__NETCDF4CXX_WRITE_VAR(unsigned short, ushort)
__NETCDF4CXX_WRITE_VAR(int, int)
__NETCDF4CXX_WRITE_VAR(unsigned int, uint)
__NETCDF4CXX_WRITE_VAR(long long, longlong)
__NETCDF4CXX_WRITE_VAR(unsigned long long, ulonglong)
__NETCDF4CXX_WRITE_VAR(float, float)
__NETCDF4CXX_WRITE_VAR(double, double)

void Variable::Copy(const Group& other) const {
  std::list<Attribute> attributes = GetAttributes();
  std::vector<Dimension> dimensions = GetDimensions();

  Variable target(other.AddVariable(GetShortName(), GetDataType(), dimensions));
  for (auto& item : GetAttributes()) {
    item.Copy(target);
  }
  std::vector<void*> buffer(GetSize() * GetDataType().GetSize());
  Hyperslab hyperslab(GetShape());
  Check(nc_get_vara(nc_id_, id_, &hyperslab.start()[0],
                    &hyperslab.GetSizeList()[0], &buffer[0]));
  Check(nc_put_vara(target.nc_id(), target.id(), &hyperslab.start()[0],
                    &hyperslab.GetSizeList()[0], &buffer[0]));
}
}
