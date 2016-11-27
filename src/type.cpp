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


#include <netcdf4_cxx/type.hpp>
#include <stdexcept>

namespace netcdf {

type::Primitive type::Generic::GetPrimitive() const {
  switch (id_) {
    case NC_BYTE:
      return Primitive::kByte;
    case NC_UBYTE:
      return Primitive::kUByte;
    case NC_CHAR:
      return Primitive::kChar;
    case NC_SHORT:
      return Primitive::kShort;
    case NC_USHORT:
      return Primitive::kUShort;
    case NC_INT:
      return Primitive::kInt;
    case NC_UINT:
      return Primitive::kUInt;
    case NC_INT64:
      return Primitive::kInt64;
    case NC_UINT64:
      return Primitive::kUInt64;
    case NC_FLOAT:
      return Primitive::kFloat;
    case NC_DOUBLE:
      return Primitive::kDouble;
    case NC_STRING:
      return Primitive::kString;
    default:
      int type;
      Check(
          nc_inq_user_type(nc_id_, id_, nullptr, nullptr, nullptr, nullptr,
                           &type));
      return static_cast<type::Primitive>(type);
  }
}

std::string type::Generic::GetPrimitiveName() const {
  type::Primitive type = GetPrimitive();
  switch (type) {
    case Primitive::kByte:
      return "byte";
    case Primitive::kUByte:
      return "unsigned byte";
    case Primitive::kChar:
      return "char";
    case Primitive::kShort:
      return "short";
    case Primitive::kUShort:
      return "unsigned short";
    case Primitive::kInt:
      return "int";
    case Primitive::kUInt:
      return "unsigned int";
    case Primitive::kInt64:
      return "long long";
    case Primitive::kUInt64:
      return "unsigned long long";
    case Primitive::kFloat:
      return "float";
    case Primitive::kDouble:
      return "double";
    case Primitive::kString:
      return "string";
    case Primitive::kVLen:
      return "vlen";
    case Primitive::kOpaque:
      return "opaque";
    case Primitive::kEnum:
      return "enum";
    case Primitive::kCompound:
      return "compound";
    default:
      throw std::runtime_error("unreachable");
  }
}

type::Enum type::Generic::GetEnum() const {
  if (GetPrimitive() != Primitive::kEnum)
    std::logic_error(
        GetName() + " is not an enumeration type");
  return Enum(*this, id_);
}

type::VLen type::Generic::GetVLen() const {
  if (GetPrimitive() != Primitive::kVLen)
    std::logic_error(
        GetName() + " is not a variable length type");
  return VLen(*this, id_);
}

type::Opaque type::Generic::GetOpaque() const {
  if (GetPrimitive() != Primitive::kOpaque)
    std::logic_error(
        GetName() + " is not an opaque type");
  return Opaque(*this, id_);
}

type::Compound type::Generic::GetCompound() const {
  if (GetPrimitive() != Primitive::kCompound)
    std::logic_error(
        GetName() + " is not a compound type");
  return Compound(*this, id_);
}

void type::Generic::Copy(const Object& target) const {
  type::Primitive type = GetPrimitive();

  switch (type) {
    case Primitive::kEnum:
      GetEnum().Copy(target);
      break;
    case Primitive::kVLen:
      GetVLen().Copy(target);
      break;
    case Primitive::kOpaque:
      GetOpaque().Copy(target);
      break;
    case Primitive::kCompound:
      GetCompound().Copy(target);
      break;
    default:
      break;
  }
}

void type::Enum::Copy(const Object& other) const {
  type::Primitive type = GetBaseType().GetPrimitive();
  type::Enum target(other, GetName(), type);
  size_t num_member = GetNMembers();

  switch (type) {
    case Primitive::kByte:
    case Primitive::kUByte: {
      for (size_t ix = 0; ix < num_member; ++ix) {
        target.Insert<signed char>(Value<signed char>(ix));
      }
    }
      break;
    case Primitive::kUShort:
    case Primitive::kShort: {
      for (size_t ix = 0; ix < num_member; ++ix) {
        target.Insert<short>(Value<short>(ix));
      }
    }
      break;
    case Primitive::kInt:
    case Primitive::kUInt: {
      for (size_t ix = 0; ix < num_member; ++ix) {
        target.Insert<int>(Value<int>(ix));
      }
    }
      break;
    case Primitive::kInt64:
    case Primitive::kUInt64: {
      for (size_t ix = 0; ix < num_member; ++ix) {
        target.Insert<long long>(Value<long long>(ix));
      }
    }
      break;
    case Primitive::kChar:
    case Primitive::kFloat:
    case Primitive::kDouble:
    case Primitive::kString:
    case Primitive::kVLen:
    case Primitive::kOpaque:
    case Primitive::kEnum:
    case Primitive::kCompound:
    default:
      throw std::runtime_error("unreachable");
  }
}

void type::Compound::Copy(const Object& target) const {
  type::Compound copy(target, GetName(), GetSize());

  for (size_t ix = 0; ix < GetNMembers(); ++ix) {
    std::vector<int> dims = GetDims(ix);
    copy.InsertMember(GetMemberName(ix), GetMemberOffset(ix),
                      GetMemberClass(ix), GetDims(ix));
  }
}


}  // namespace netcdf
