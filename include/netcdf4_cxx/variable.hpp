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
#include <functional>
#include <iostream>
#include <netcdf4_cxx/dataset.hpp>
#include <netcdf4_cxx/dimension.hpp>
#include <netcdf4_cxx/hyperslab.hpp>
#include <netcdf4_cxx/netcdf.hpp>
#include <netcdf4_cxx/type.hpp>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

namespace netcdf {

/**
 * A Variable is a logical container for data. It has a dataType, a set of
 * Dimensions that define its array shape, and optionally a set of Attributes.
 */
class Variable : public DataSet {
 public:
  /**
   * Default constructor
   *
   * @param object NetCDF Object
   * @param var_id variable ID
   */
  Variable(const Object& object, const int var_id) : DataSet(object, var_id) {}

  /**
   * Get the data type of the Variable.
   *
   * @return data type
   */
  type::Generic GetDataType() const {
    nc_type result;
    Check(nc_inq_vartype(nc_id_, id_, &result));
    return type::Generic(*this, result);
  }

  /**
   * Get the number of dimensions of the Variable.
   *
   * @return the rank
   */
  inline size_t GetRank() const {
    int result;
    Check(nc_inq_varndims(nc_id_, id_, &result));
    return static_cast<size_t>(result);
  }

  /**
   * Get the list of dimensions used by this variable.
   *
   * @return list of dimensions
   */
  std::vector<Dimension> GetDimensions() const;

  /**
   * Get the shape: length of Variable in each dimension
   *
   * @return array whose length is the rank of this variable and whose
   * values equal the length of that Dimension.
   */
  std::vector<size_t> GetShape() const {
    std::vector<size_t> result;
    for (auto& item : GetDimensions()) {
      result.push_back(item.GetLength());
    }
    return result;
  }

  /**
   * Get the total number of elements in the Variable.
   *
   * @return the total number of elements in the Variable
   */
  size_t GetSize() const {
    std::vector<size_t> shape = GetShape();
    return std::accumulate(shape.begin(), shape.end(), 1,
                           std::multiplies<size_t>());
  }

  /**
   * Get the short name of this Dimension.
   *
   * @return the name
   */
  inline std::string GetShortName() const {
    char result[NC_MAX_NAME + 1];
    Check(nc_inq_varname(nc_id_, id_, result));
    return result;
  }

  /**
   * Get the long name of this Dimension.
   */
  std::string GetLongName() const;

  /**
   * Copy the variable from the netCDF dataset to another
   *
   * @param other target group
   */
  void Copy(const Group& other) const;

  /**
   * Calculate if this is a classic coordinate variable: has same name as
   * its first dimension. If type char, must be 2D, else must be 1D.
   *
   * @return true if a coordinate variable.
   */
  bool IsCoordinate() const;

  /**
   * Can this variable's size grow?. This is equivalent to saying at least
   * one of its dimensions is unlimited.
   *
   * @return true if this variable can grow
   */
  bool IsUnlimited() const {
    for (auto& item : GetDimensions()) {
      if (item.IsUnlimited()) return true;
    }
    return false;
  }

  /**
   * Set chunk cache
   *
   * @param size The total size of the raw data chunk cache, in bytes.
   * @param slots The number of chunk slots in the raw data chunk cache.
   * @param preemption The preemption, a value between 0 and 1 inclusive that
   *      indicates how much chunks that have been fully read are favored for
   *      preemption. A value of zero means fully read chunks are treated no
   *      differently than other chunks (the preemption is strictly LRU)
   *      while a value of one means fully read chunks are always preempted
   *      before other chunks.
   */
  inline void SetChunkCache(const size_t size, const size_t slots,
                            const float preemption) const {
    Check(nc_set_chunk_cache(size, slots, preemption));
  }

  /**
   * Get chunk cache
   *
   * @param size The total size of the raw data chunk cache, in bytes.
   * @param slots The number of chunk slots in the raw data chunk cache.
   * @param preemption The preemption, a value between 0 and 1 inclusive that
   *      indicates how much chunks that have been fully read are favored for
   *      preemption. A value of zero means fully read chunks are treated no
   *      differently than other chunks (the preemption is strictly LRU)
   *      while a value of one means fully read chunks are always preempted
   *      before other chunks.
   */
  inline void GetChunkCache(size_t& size, size_t& slots,
                            float& preemption) const {
    Check(nc_get_chunk_cache(&size, &slots, &preemption));
  }

  /**
   * Rename a variable
   *
   * @param name new name
   */
  inline void Rename(const std::string& name) const {
    Check(nc_rename_var(nc_id_, id_, name.c_str()));
  }

  /**
   * Read the data for this Variable
   *
   * @param hyperslab Hyperslabs to be read
   * @param values data read
   * @return a reference on the data read
   */
  template <class T>
  std::vector<T> Read(const Hyperslab& hyperslab) const {
    if (sizeof(T) != GetDataType().GetSize())
      throw std::invalid_argument(
          "the size of the NetCDF type does not "
          "match the size of the given C++ type");

    if (hyperslab > GetShape())
      throw std::invalid_argument(
          "Hyperslab defined overlap the "
          "variable definition");

    std::vector<T> values(hyperslab.GetSize());

    if (hyperslab.OnlyAdjacent())
      Check(nc_get_vara(nc_id_, id_, &hyperslab.start()[0],
                        &hyperslab.GetSizeList()[0], &values[0]));
    else
      Check(nc_get_vars(nc_id_, id_, &hyperslab.start()[0],
                        &hyperslab.GetSizeList()[0], &hyperslab.step()[0],
                        &values[0]));
    return values;
  }

  /**
   * Read all the data for this Variable
   *
   * @param values data read
   * @return a reference on the data read
   */
  template <class T>
  std::vector<T> Read() const {
    return Read<T>(Hyperslab(GetShape()));
  }

  /**
   * Write data for this variable
   *
   * @param hyperslab Hyperslabs to be write
   * @param values values to write
   */
  template <typename T>
  void Write(const Hyperslab& hyperslab, const std::vector<T>& values) const {
    if (sizeof(T) != GetDataType().GetSize())
      throw std::invalid_argument(
          "the size of the NetCDF type does not "
          "match the size of the given C++ type");

    if (hyperslab.IsEmpty()) {
      // Avoid the problems dealing with the writing a new unlimited variables
      if (IsUnlimited())
        throw std::runtime_error(
            "You must specify a hyperslab for "
            "unlimited variables");

      Check(nc_put_var(nc_id_, id_, &values[0]));
    } else {
      if (values.size() != hyperslab.GetSize())
        throw std::invalid_argument(
            "data size does not match hyperslab "
            "definition");
      if (hyperslab.OnlyAdjacent())
        Check(nc_put_vara(nc_id_, id_, &hyperslab.start()[0],
                          &hyperslab.GetSizeList()[0], &values[0]));
      else
        Check(nc_put_vars(nc_id_, id_, &hyperslab.start()[0],
                          &hyperslab.GetSizeList()[0], &hyperslab.step()[0],
                          &values[0]));
    }
  }

#define _NETCDF4CXX_WRITE_VAR(type) \
  void Write(const Hyperslab& hyperslab, const std::vector<type>& values) const;

  _NETCDF4CXX_WRITE_VAR(signed char)
  _NETCDF4CXX_WRITE_VAR(unsigned char)
  _NETCDF4CXX_WRITE_VAR(short)
  _NETCDF4CXX_WRITE_VAR(unsigned short)
  _NETCDF4CXX_WRITE_VAR(int)
  _NETCDF4CXX_WRITE_VAR(unsigned int)
  _NETCDF4CXX_WRITE_VAR(long long)
  _NETCDF4CXX_WRITE_VAR(unsigned long long)
  _NETCDF4CXX_WRITE_VAR(float)
  _NETCDF4CXX_WRITE_VAR(double)

  /**
   * Write all data for this variable
   *
   * @param values values to write
   */
  template <class T>
  void Write(std::vector<T>& values) const {
    Write(Hyperslab(), values);
  }
};

#define _NETCDF4CXX_READ_VAR(type) \
  template <>                      \
  std::vector<type> Variable::Read(const Hyperslab& hyperslab) const;

_NETCDF4CXX_READ_VAR(signed char)
_NETCDF4CXX_READ_VAR(unsigned char)
_NETCDF4CXX_READ_VAR(short)
_NETCDF4CXX_READ_VAR(unsigned short)
_NETCDF4CXX_READ_VAR(int)
_NETCDF4CXX_READ_VAR(unsigned int)
_NETCDF4CXX_READ_VAR(long long)
_NETCDF4CXX_READ_VAR(unsigned long long)
_NETCDF4CXX_READ_VAR(float)
_NETCDF4CXX_READ_VAR(double)

}  // namespace netcdf
