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
#include <stddef.h>
#include <iostream>
#include <netcdf4_cxx/abstract_dataset.hpp>
#include <netcdf4_cxx/netcdf.hpp>
#include <netcdf4_cxx/object.hpp>
#include <netcdf4_cxx/type.hpp>
#include <stdexcept>
#include <string>
#include <vector>

namespace netcdf {

/**
 * An Attribute has a name and a value, used for associating arbitrary metadata
 * with a Variable or a Group. The value can be a one dimensional array of
 * strings or numeric values.
 */
class Attribute : public AbstractDataSet {
 public:
  /**
   * Create a global Attribute
   *
   * @param object NetCDF Object
   * @param name Attribute name
   */
  explicit Attribute(const Object& object, const std::string& name)
      : AbstractDataSet(object), name_(name) {}

  /**
   * Create an Attribute to an existing Variable
   *
   * @param dataset Existing data set
   * @param name Attribute name
   */
  explicit Attribute(const AbstractDataSet& dataset, const std::string& name)
      : AbstractDataSet(dataset), name_(name) {}

  /**
   * Copy constructor
   *
   * @param rhs right hand side
   */
  Attribute(const Attribute& rhs) : AbstractDataSet(rhs), name_(rhs.name_) {}

  /**
   * Move constructor
   *
   * @param rhs right hand side
   */
  Attribute(Attribute&& rhs)
      : AbstractDataSet(rhs), name_(std::move(rhs.name_)) {}

  /**
   * Move assignment operator
   *
   * @param rhs right hand side
   */
  Attribute& operator=(Attribute&& rhs) {
    if (this != &rhs) {
      AbstractDataSet::operator=(std::move(rhs));
      name_ = std::move(rhs.name_);
    }
    return *this;
  }

  /**
   * Write data to this attribute.
   *
   * @param data_type Attribute type (in memory)
   * @param data Data to be written
   */
  template <typename T>
  void Write(const type::Generic& data_type, const std::vector<T>& data) const {
    if (sizeof(T) != data_type.GetSize())
      throw std::invalid_argument(
          "the size of the NetCDF type does not "
          "match the size of the given C++ type");
    Check(nc_put_att(nc_id_, id_, name_.c_str(), data_type.id(), data.size(),
                     &data[0]));
  }

#define _NETCDF4CXX_WRITE_ATT(_type, _sufix)                                 \
  void Write(const type::Generic& data_type, const std::vector<_type>& data) \
      const {                                                                \
    Check(nc_put_att_##_sufix(nc_id_, id_, name_.c_str(), data_type.id(),    \
                              data.size(), &data[0]));                       \
  }

  _NETCDF4CXX_WRITE_ATT(signed char, schar)
  _NETCDF4CXX_WRITE_ATT(short, short)
  _NETCDF4CXX_WRITE_ATT(unsigned char, uchar)
  _NETCDF4CXX_WRITE_ATT(unsigned short, ushort)
  _NETCDF4CXX_WRITE_ATT(int, int)
  _NETCDF4CXX_WRITE_ATT(unsigned int, uint)
  _NETCDF4CXX_WRITE_ATT(long long, longlong)
  _NETCDF4CXX_WRITE_ATT(unsigned long long, ulonglong)
  _NETCDF4CXX_WRITE_ATT(float, float)
  _NETCDF4CXX_WRITE_ATT(double, double)

  /**
  * Write a text (ASCII/ISO characters) Attribute
  *
  * @param data Text attribute
  */
  void WriteText(const std::string& data) const {
    Check(
        nc_put_att_text(nc_id_, id_, name_.c_str(), data.size(), data.c_str()));
  }

  /**
   * Write a string Attribute
   *
   * @param data String attribute
   */
  void WriteString(const std::vector<std::string>& data) const {
    std::vector<const char*> buffer;

    for (auto& item : data) {
      buffer.push_back(item.c_str());
    }
    Check(
        nc_put_att_string(nc_id_, id_, name_.c_str(), data.size(), &buffer[0]));
  }

  /**
   * Write a string Attribute
   *
   * @param data String attribute
   */
  void WriteString(const std::string& data) const {
    WriteString(std::vector<std::string>{data});
  }

  /**
   * Get the data type of the Attribute value.
   *
   * @return the data type
   */
  type::Generic GetDataType() const {
    nc_type type_id;
    Check(nc_inq_atttype(nc_id_, id_, name_.c_str(), &type_id));
    return type::Generic(*this, type_id);
  }

  /**
   * True if value is of type text.
   *
   * @return if it's a string or not
   */
  bool IsText() const {
    type::Primitive data_type = GetDataType().GetPrimitive();
    return data_type == type::Primitive::kChar;
  }

  /**
   * True if value is of type string.
   *
   * @return if it's a string or not
   */
  bool IsString() const {
    type::Primitive data_type = GetDataType().GetPrimitive();
    return data_type == type::Primitive::kString;
  }

  /**
   * Get the length of the attribute
   *
   * @return the number of values currently stored in the attribute
   */
  size_t GetLength() const {
    size_t result;
    Check(nc_inq_attlen(nc_id_, id_, name_.c_str(), &result));
    return result;
  }

  /**
   * Get attribute name
   *
   * @return the attribute name
   */
  const std::string& name() const noexcept { return name_; }

  /**
   * Copy the attribute from the netCDF object to an other
   *
   * @param other The NetCDF object target of the copy
   */
  void Copy(const AbstractDataSet& other) const {
    Check(nc_copy_att(nc_id_, id_, name_.c_str(), other.nc_id(), other.id()));
  }

  /**
   * Rename the attribute
   *
   * @param name the new attribute name
   */
  void Rename(const std::string& name) {
    Check(nc_rename_att(nc_id_, id_, name_.c_str(), name.c_str()));
    name_ = name;
  }

  /**
   * Read the value as an array.
   *
   * @return a new container on values read
   */
  template <typename T>
  std::vector<T> Read() const {
    if (sizeof(T) != GetDataType().GetSize())
      throw std::invalid_argument(
          "the size of the NetCDF type does not "
          "match the size of the given C++ type");
    std::vector<T> values(GetLength());
    Check(nc_get_att(nc_id_, id_, name_.c_str(), &values[0]));
    return values;
  }

  /**
   * Read the scalar value of an attribute.
   *
   * @return the value of the attribute
   */
  template <typename T>
  decltype(auto) ReadScalar() const {
    std::vector<T> values = Read<T>();
    return values.at(0);
  }

  /**
   * Retrieve text value; only call if IsText() is true.
   *
   * @return text read
   */
  std::string ReadText() const {
    std::string result(GetLength(), 0);
    Check(nc_get_att_text(nc_id_, id_, name_.c_str(), &result[0]));
    return result;
  }

  /**
   * Retrieve String values; only call if IsString() is true.
   *
   * @return strings read
   */
  std::vector<std::string> ReadString() const;

 private:
  std::string name_;
};

#define _NETCDF4CXX_READ_ATT(_type) \
  template <>                       \
  std::vector<_type> Attribute::Read() const;

_NETCDF4CXX_READ_ATT(signed char)
_NETCDF4CXX_READ_ATT(unsigned char)
_NETCDF4CXX_READ_ATT(short)
_NETCDF4CXX_READ_ATT(unsigned short)
_NETCDF4CXX_READ_ATT(int)
_NETCDF4CXX_READ_ATT(unsigned int)
_NETCDF4CXX_READ_ATT(long long)
_NETCDF4CXX_READ_ATT(unsigned long long)
_NETCDF4CXX_READ_ATT(float)
_NETCDF4CXX_READ_ATT(double)

}  // namespace netcdf
