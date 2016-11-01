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


#include <netcdf/type.hpp>
#include <stdexcept>

namespace netcdf {

type::Primitive type::Generic::GetPrimitive() const {
  switch (id_) {
    case NC_BYTE:
      return kByte;
    case NC_UBYTE:
      return kUByte;
    case NC_CHAR:
      return kChar;
    case NC_SHORT:
      return kShort;
    case NC_USHORT:
      return kUShort;
    case NC_INT:
      return kInt;
    case NC_UINT:
      return kUInt;
    case NC_INT64:
      return kInt64;
    case NC_UINT64:
      return kUInt64;
    case NC_FLOAT:
      return kFloat;
    case NC_DOUBLE:
      return kDouble;
    case NC_STRING:
      return kString;
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
    case kByte:
      return "byte";
    case kUByte:
      return "unsigned byte";
    case kChar:
      return "char";
    case kShort:
      return "short";
    case kUShort:
      return "unsigned short";
    case kInt:
      return "int";
    case kUInt:
      return "unsigned int";
    case kInt64:
      return "long long";
    case kUInt64:
      return "unsigned long long";
    case kFloat:
      return "float";
    case kDouble:
      return "double";
    case kString:
      return "string";
    case kVLen:
      return "vlen";
    case kOpaque:
      return "opaque";
    case kEnum:
      return "enum";
    case kCompound:
      return "compound";
    default:
      throw std::runtime_error("unreachable");
  }
}

type::Enum type::Generic::GetEnum() const {
  if (GetPrimitive() != kEnum)
    std::logic_error(
        GetName() + " is not an enumeration type");
  return Enum(*this, id_);
}

type::VLen type::Generic::GetVLen() const {
  if (GetPrimitive() != kVLen)
    std::logic_error(
        GetName() + " is not a variable length type");
  return VLen(*this, id_);
}

type::Opaque type::Generic::GetOpaque() const {
  if (GetPrimitive() != kOpaque)
    std::logic_error(
        GetName() + " is not an opaque type");
  return Opaque(*this, id_);
}

type::Compound type::Generic::GetCompound() const {
  if (GetPrimitive() != kCompound)
    std::logic_error(
        GetName() + " is not a compound type");
  return Compound(*this, id_);
}

void type::Generic::Copy(const Object& target) const {
  type::Primitive type = GetPrimitive();

  switch (type) {
    case kEnum:
      GetEnum().Copy(target);
      break;
    case kVLen:
      GetVLen().Copy(target);
      break;
    case kOpaque:
      GetOpaque().Copy(target);
      break;
    case kCompound:
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
    case kByte:
    case kUByte: {
      for (size_t ix = 0; ix < num_member; ++ix) {
        target.Insert<signed char>(Value<signed char>(ix));
      }
    }
      break;
    case kUShort:
    case kShort: {
      for (size_t ix = 0; ix < num_member; ++ix) {
        target.Insert<short>(Value<short>(ix));
      }
    }
      break;
    case kInt:
    case kUInt: {
      for (size_t ix = 0; ix < num_member; ++ix) {
        target.Insert<int>(Value<int>(ix));
      }
    }
      break;
    case kInt64:
    case kUInt64: {
      for (size_t ix = 0; ix < num_member; ++ix) {
        target.Insert<long long>(Value<long long>(ix));
      }
    }
      break;
    case kChar:
    case kFloat:
    case kDouble:
    case kString:
    case kVLen:
    case kOpaque:
    case kEnum:
    case kCompound:
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
