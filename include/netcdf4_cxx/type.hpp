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

#include <netcdf4_cxx/netcdf.hpp>
#include <netcdf4_cxx/object.hpp>
#include <netcdf.h>
#include <stddef.h>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace netcdf {
namespace type {

/**
 * netCDF primitive data type
 */
enum class Primitive {
  kNotAType = NC_NAT,       //!< kNotAType not a type
  kByte = NC_BYTE,          //!< kByte signed 1 byte integer
  kChar = NC_CHAR,          //!< kChar ISO/ASCII character
  kShort = NC_SHORT,        //!< kShort signed 2 bytes integer
  kInt = NC_INT,            //!< kInt signed 4 bytes integer
  kFloat = NC_FLOAT,        //!< kFloat single precision floating point number
  kDouble = NC_DOUBLE,      //!< kDouble double precision floating point number
  kUByte = NC_UBYTE,        //!< kUByte unsigned 1 byte integer
  kUShort = NC_USHORT,      //!< kUShort unsigned 2 bytes integer
  kUInt = NC_UINT,          //!< kUInt unsigned 4 bytes integer
  kInt64 = NC_INT64,        //!< kInt64 signed 8 bytes integer
  kUInt64 = NC_UINT64,      //!< kUInt64 unsigned 8 bytes integer
  kString = NC_STRING,      //!< kString string
  kVLen = NC_VLEN,          //!< kVLen variable length types
  kOpaque = NC_OPAQUE,      //!< kOpaque opaque types
  kEnum = NC_ENUM,          //!< kEnum enum types
  kCompound = NC_COMPOUND,  //!< kCompound compound types
};

class Enum;
class VLen;
class Opaque;
class Compound;

/**
 * Generic data type in netCDF File
 */
class Generic : public Object {
 protected:
  nc_type id_;  //!< Type ID

 public:
  /**
   * Default constructor
   *
   * @param object NetCDF Object
   * @param type Type ID
   */
  explicit Generic(const Object& object,
                   const nc_type type)
      : Object(object),
        id_(type) {
  }

  /**
   * Builds a new data type
   *
   * @param object NetCDF Object
   * @param type Type ID
   */
  explicit Generic(const Object& object,
                   const Primitive type)
      : Object(object),
        id_(static_cast<int>(type)) {
  }

  /**
   * Destructor
   */
  virtual ~Generic() {
  }

  /**
   * Get the netCDF type ID
   *
   * @return the netCDF type ID
   */
  nc_type id() const {
    return id_;
  }

  /**
   * Get the name of this type
   *
   * @return the type name
   */
  std::string GetName() const {
    char result[NC_MAX_NAME + 1];
    Check(nc_inq_type(nc_id_, id_, result, nullptr));
    return result;
  }

  /**
   * Get the (in-memory) size of the type
   *
   * @return the size in bytes
   */
  size_t GetSize() const {
    size_t result;
    Check(nc_inq_type(nc_id_, id_, nullptr, &result));
    return result;
  }

  /**
   * Get data type or the class of the user defined type
   *
   * @return the data type
   */
  Primitive GetPrimitive() const;

  /**
   * Get data type name or the name of the class of the user defined type
   *
   * @return the type name
   */
  std::string GetPrimitiveName() const;

  /**
   * Is this an enumeration type?
   *
   * @return true if enumerate type
   */
  bool IsEnum() const {
    return GetPrimitive() == Primitive::kEnum;
  }

  /**
   * Is this a compound type?
   *
   * @return true if compound type
   */
  bool IsCompound() const {
    return GetPrimitive() == Primitive::kCompound;
  }

  /**
   * Is this a variable length type?
   *
   * @return true if variable length type
   */
  bool IsVLen() const {
    return GetPrimitive() == Primitive::kVLen;
  }

  /**
   * Is this an opaque type?
   *
   * @return true if opaque type
   */
  bool IsOpaque() const {
    return GetPrimitive() == Primitive::kOpaque;
  }

  /**
   * Is Float or Double
   *
   * @return true if floating point type
   */
  bool IsFloatingPoint() const {
    return id_ == NC_FLOAT || id_ == NC_DOUBLE;
  }

  /**
   * Is byte, short, int or long
   *
   * @return true if integral
   */
  bool IsIntegral() const {
    return id_ == NC_BYTE || id_ == NC_UBYTE || id_ == NC_SHORT
        || id_ == NC_USHORT || id_ == NC_INT || id_ == NC_UINT
        || id_ == NC_INT64 || id_ == NC_UINT64;
  }

  /**
   * Is byte, float, double, int, short, or long
   *
   * @return true if numeric
   */
  bool IsNumeric() const {
    return IsFloatingPoint() || IsIntegral();
  }

  /**
   * Is string or char
   *
   * @return if string or char
   */
  bool IsString() const {
    return id_ == NC_STRING || id_ == NC_CHAR;
  }

  /**
   * Is this a primitive type
   *
   * @return true if primitive type
   */
  bool IsPrimitive() const {
    return id_ <= NC_STRING;
  }

  /**
   * Is this a user data type
   *
   * @return true if user type
   */
  bool IsUserType() const {
    return id_ > NC_STRING;
  }

  /**
   * Copy this data type to an on other NetCDF Object
   *
   * @param target NetCDF Object
   */
  virtual void Copy(const Object& target) const;

  /**
   * Promote this instance the an Enum data type.
   *
   * @return Enum data type
   * @throw std::logic_error if the instance is not an Enum type
   */
  Enum GetEnum() const;

  /**
   * Promote this instance the a VLen data type.
   *
   * @return VLen data type
   * @throw std::logic_error if the instance is not a VLen type
   */
  VLen GetVLen() const;

  /**
   * Promote this instance the an Opaque data type.
   *
   * @return Opaque data type
   * @throw std::logic_error if the instance is not an Opaque type
   */
  Opaque GetOpaque() const;

  /**
   * Promote this instance the a Compound data type.
   *
   * @return Compound data type
   * @throw std::logic_error if the instance is not a Compound type
   */
  Compound GetCompound() const;

  /**
   * Test if two NetCDF data set are different
   *
   * @param rhs Other data set to compare
   * @return true if the two instances are different
   */
  bool operator !=(const Object& rhs) const override {
    const Generic* inherited = dynamic_cast<const Generic*>(&rhs);
    if (inherited != nullptr) {
      return Object::operator !=(rhs) or id_ != inherited->id_;
    }
    return true;
  }

  /**
   * Test if two NetCDF data set are equal
   *
   * @param rhs Other data set to compare
   * @return true if the two instances are equal
   */
  bool operator ==(const Object& rhs) const override {
    const Generic* inherited = dynamic_cast<const Generic*>(&rhs);
    if (inherited != nullptr) {
      return Object::operator ==(rhs) and id_ == inherited->id_;
    }
    return false;
  }
};

/**
 * Signed 1 byte integer
 */
class Byte : public Generic {
 public:
  /**
   * Default constructor
   */
  Byte(const Object& object)
      : Generic(object, NC_BYTE) {
  }
};

/**
 * ISO/ASCII character
 */
class Char : public Generic {
 public:
  /**
   * Default constructor
   */
  Char(const Object& object)
      : Generic(object, NC_CHAR) {
  }
};

/**
 * signed 2 bytes integer
 */
class Short : public Generic {
 public:
  /**
   * Default constructor
   */
  Short(const Object& object)
      : Generic(object, NC_SHORT) {
  }
};

/**
 * signed 4 bytes integer
 */
class Int : public Generic {
 public:
  /**
   * Default constructor
   */
  Int(const Object& object)
      : Generic(object, NC_INT) {
  }
};

/**
 * Single precision floating point number
 */
class Float : public Generic {
 public:
  /**
   * Default constructor
   */
  Float(const Object& object)
      : Generic(object, NC_FLOAT) {
  }
};

/**
 * Double precision floating point number
 */
class Double : public Generic {
 public:
  /**
   * Default constructor
   */
  Double(const Object& object)
      : Generic(object, NC_DOUBLE) {
  }
};

/**
 * signed 8 bytes integer
 */
class Int64 : public Generic {
 public:
  /**
   * Default constructor
   */
  Int64(const Object& object)
      : Generic(object, NC_INT64) {
  }
};

/**
 * Unsigned 1 byte integer
 */
class UnsignedByte : public Generic {
 public:
  /**
   * Default constructor
   */
  UnsignedByte(const Object& object)
      : Generic(object, NC_UBYTE) {
  }
};

/**
 * Unsigned 2 bytes integer
 */
class UnsignedShort : public Generic {
 public:
  /**
   * Default constructor
   */
  UnsignedShort(const Object& object)
      : Generic(object, NC_USHORT) {
  }
};

/**
 * Unsigned 4 bytes integer
 */
class UnsignedInt : public Generic {
 public:
  /**
   * Default constructor
   */
  UnsignedInt(const Object& object)
      : Generic(object, NC_UINT) {
  }
};

/**
 * Unsigned 8 bytes integer
 */
class UnsignedInt64 : public Generic {
 public:
  /**
   * Default constructor
   */
  UnsignedInt64(const Object& object)
      : Generic(object, NC_UINT64) {
  }
};

/**
 * String
 */
class String : public Generic {
 public:
  /**
   * Default constructor
   */
  String(const Object& object)
      : Generic(object, NC_STRING) {
  }
};

/**
 * NetCDF Enum type
 */
class Enum : public Generic {
 public:
  /**
   * Default constructor
   *
   * @param object NetCDF Object
   * @param type Type ID
   */
  explicit Enum(const Object& object,
                const nc_type type)
      : Generic(object, type) {
  }

  /**
   * Create a new Enum type
   *
   * @param object NetCDF Object
   * @param base_type The base integer type for this enum. Must be one of:
   *  kByte, kUbyte, kShort, kUShort, kInt, kUInt, kInt64, kUInt64.
   * @param name NetCDF names of new type.
   */
  explicit Enum(const Object& object,
                const std::string& name,
                const Primitive base_type)
      : Generic(object, Primitive::kNotAType) {
    Check(nc_def_enum(nc_id_, static_cast<int>(base_type), name.c_str(), &id_));
  }

  /**
   * Inserts a new member to this Enum type.
   *
   * @param member Member properties
   */
  template<typename T>
  void Insert(const std::pair<std::string, T>& member) const {
    Check(
        nc_insert_enum(nc_id_, id_, member.first.c_str(),
                       static_cast<const void*>(&member.second)));
  }

  /**
   * Get the type of the Enum base type.
   *
   * @return the base type
   */
  Generic GetBaseType() const {
    nc_type result;
    Check(nc_inq_enum(nc_id_, id_, nullptr, &result, nullptr, nullptr));
    return Generic(*this, result);
  }

  /**
   * Copy this data type to an on other NetCDF Object
   *
   * @param target NetCDF Object
   */
  void Copy(const Object& target) const override;

  /**
   * Get the number of members defined for this Enum type
   *
   * @return the number of members
   */
  size_t GetNMembers() const {
    size_t result;
    Check(nc_inq_enum(nc_id_, id_, nullptr, nullptr, nullptr, &result));
    return result;
  }

  /**
   * Get the pair of values between the NetCDF name of the member and the value
   * associated with this member
   *
   * @param index Member index
   * @return the pair
   */
  template<typename T>
  std::pair<std::string, T> Value(const int index) const {
    char name[NC_MAX_NAME + 1];
    T value;

    Check(
        nc_inq_enum_member(nc_id_, id_, index, name,
                           static_cast<void*>(&value)));

    return std::pair<std::string, T>(name, value);
  }

  /**
   * Get the associative array between the identifier (NetCDF names) and
   * the value for this enum type
   *
   * @return a hash containing the constants and values of this enum type
   */
  template<typename T>
  std::map<std::string, T> Values() const {
    std::map<std::string, T> result;

    for (size_t index = 0; index < GetNMembers(); ++index) {
      result.insert(Value<T>(index));
    }

    return result;
  }
};

/**
 * NetCDF VLen (variable-length) type
 */
class VLen : public Generic {
 public:
  /**
   * Default constructor
   *
   * @param object NetCDF Object
   * @param type Type ID
   */
  explicit VLen(const Object& object,
                const nc_type type)
      : Generic(object, type) {
  }

  /**
   * Create a new compound type
   *
   * @param object NetCDF Object
   * @param name NetCDF names of the created type
   * @param type The type of the base type of the VLEN.
   */
  explicit VLen(const Object& object,
                const std::string& name,
                const Generic& type)
      : Generic(object, Primitive::kNotAType) {
    Check(nc_def_vlen(nc_id_, name.c_str(), type.id(), &id_));
  }

  /**
   * Get the type of the VLen base type
   *
   * @return the base type
   */
  Generic GetBaseType() const {
    nc_type result;
    Check(nc_inq_vlen(nc_id_, id_, nullptr, nullptr, &result));
    return Generic(Object(*this), result);
  }

  /**
   * Copy this data type to an on other NetCDF Object
   *
   * @param target NetCDF Object
   */
  void Copy(const Object& target) const override {
    type::VLen(target, GetName(), GetBaseType());
  }
};

/**
 * NetCDF Opaque type
 */
class Opaque : public Generic {
 public:
  /**
   * Default constructor
   *
   * @param object NetCDF Object
   * @param type type ID
   */
  explicit Opaque(const Object& object,
                  const nc_type type)
      : Generic(object, type) {
  }

  /**
   * Create a new Opaque type
   *
   * @param object NetCDF Object
   * @param name NetCDF names of the new type
   * @param size The size of each opaque object in bytes.
   */
  explicit Opaque(const Object& object,
                  const std::string& name,
                  const size_t size)
      : Generic(object, Primitive::kNotAType) {
    Check(nc_def_opaque(nc_id_, size, name.c_str(), &id_));
  }

  /**
   * Copy this data type to an on other NetCDF Object
   *
   * @param target NetCDF Object
   */
  void Copy(const Object& target) const override {
    type::Opaque(target, GetName(), GetSize());
  }
};

/**
 * NetCDF Compound type
 */
class Compound : public Generic {
 public:
  /**
   * Default constructor
   *
   * @param object NetCDF Object
   * @param type Type ID
   */
  explicit Compound(const Object& object,
                    const nc_type type)
      : Generic(object, type) {
  }

  /**
   * Create a new Compound type
   *
   * @param object NetCDF Object
   * @param name NetCDF names of the created type
   * @param size The size, in bytes, of the compound type
   */
  explicit Compound(const Object& object,
                    const std::string& name,
                    const size_t size)
      : Generic(object, Primitive::kNotAType) {
    Check(nc_def_compound(nc_id_, size, name.c_str(), &id_));
  }

  /**
   * Get the number of members in this Compound type.
   *
   * @return the number of members
   */
  size_t GetNMembers() const {
    size_t result;
    Check(nc_inq_compound_nfields(nc_id_, id_, &result));
    return result;
  }

  /**
   * Get the name of a member in this Compound type
   *
   * @param index Zero-based index of the member
   * @return the name of the member
   */
  std::string GetMemberName(const int index) const {
    char name[NC_MAX_NAME];
    Check(nc_inq_compound_fieldname(nc_id_, id_, index, name));
    return name;
  }

  /**
   * Get the index of a member in this Compound type
   *
   * @param name Name of the member
   * @return the index of member
   */
  int GetMemberIndex(const std::string& name) const {
    int result;
    Check(nc_inq_compound_fieldindex(nc_id_, id_, name.c_str(), &result));
    return result;
  }

  /**
   * Get the byte offset of the beginning of a member with
   * respect to the beginning of the Compound data type
   *
   * @param index Zero-based index of the member
   * @return byte offset
   */
  size_t GetMemberOffset(const int index) const {
    size_t result;
    Check(nc_inq_compound_fieldoffset(nc_id_, id_, index, &result));
    return result;
  }

  /**
   * Get the Generic type of the specified member.
   *
   * @param index Zero-based index of the member
   * @return Generic type of the member
   */
  Generic GetMemberClass(const int index) const {
    nc_type type;
    Check(nc_inq_compound_fieldtype(nc_id_, id_, index, &type));
    return Generic(*this, type);
  }

  /**
   * Get the Compound type of the specified member in this
   * Compound type.
   *
   * @param index Zero-based index of the member
   * @return Compound type instance
   */
  Compound GetMemberCompound(const int index) const {
    return Compound(*this, GetMemberClass(index).id());
  }

  /**
   * Get the Enum type of the specified member in this
   * compound type.
   *
   * @param index Zero-based index of the member
   * @return Enum type instance
   */
  Enum GetMemberEnum(const int index) const {
    return Enum(*this, GetMemberClass(index).id());
  }

  /**
   * Get the VLen type of the specified member in this Compound
   * type.
   *
   * @param index Zero-based index of the member
   * @return VLen type instance
   */
  VLen GetMemberVarLen(const int index) const {
    return VLen(*this, GetMemberClass(index).id());
  }

  /**
   * Get the Opaque type of the specified member in this Compound type
   *
   * @param index Zero-based index of the member
   * @return Opaque type instance
   */
  Opaque GetMemberOpaque(const int index) const {
    return Opaque(*this, GetMemberClass(index).id());
  }

  /**
   * Get the number of dimensions of the specified member in this Compound
   * type (scalars are 0)
   *
   * @param index Zero-based index of the member
   * @return the number of dimensions
   */
  int GetNDims(const int index) const {
    int result;
    Check(nc_inq_compound_fieldndims(nc_id_, id_, index, &result));
    return result;
  }

  /**
   * Get the sizes of dimensions of the specified member in this Compound
   * type
   *
   * @param index
   * @return the shape of the specified member or one empty vector for a
   *  scalar
   */
  std::vector<int> GetDims(const int index) const {
    int ndims = GetNDims(index);
    std::vector<int> result;

    if (ndims == 0)
      return result;

    result.resize(ndims);
    Check(nc_inq_compound_fielddim_sizes(nc_id_, id_, index, &result[0]));
    return result;
  }

  /**
   * Inserts a new member to this Compound type
   *
   * @param name Name of the new member
   * @param offset Offset in memory structure of the field to insert
   * @param type The type of the field to be inserted.
   */
  void InsertMember(const std::string& name,
                    const size_t offset,
                    const Generic& type) const {
    Check(nc_insert_compound(nc_id_, id_, name.c_str(), offset, type.id()));
  }

  /**
   * Insert a new array member to this Compound type
   *
   * @param name Name of the new member
   * @param offset Offset in memory structure of the field to insert
   * @param type The type of the field to be inserted
   * @param shape The shape of the array
   */
  void InsertMember(const std::string& name,
                    const size_t offset,
                    const Generic& type,
                    const std::vector<int>& shape) const {
    if (shape.empty())
      InsertMember(name, offset, type);
    else {
      Check(
          nc_insert_array_compound(nc_id_, id_, name.c_str(), offset, type.id(),
                                   shape.size(), const_cast<int*>(&shape[0])));
    }
  }

  /**
   * Copy this data type to an on other NetCDF Object
   *
   * @param target NetCDF Object
   */
  void Copy(const Object& target) const override;
};

}  // namespace type
}  // namespace netcdf
