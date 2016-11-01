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

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace netcdf {

/**
 * Represents a set of integers, used as an index for arrays.
 *
 * Values are generated within the half-open interval [start, stop[ (in other
 * words, the interval including start but excluding stop).
 */
class Range {
 private:
  size_t start_;    //!< start of interval
  size_t end_;      //!< end of interval
  ptrdiff_t step_;  //!< spacing between values

 public:
  /**
   * Create an empty Range.
   */
  Range()
      : start_(0),
        end_(0),
        step_(1) {
  }

  /**
   * Create a Range starting at zero, with unit step.
   *
   * @param length number of elements in the interval
   */
  Range(const size_t length)
      : start_(0),
        end_(length),
        step_(1) {
  }

  /**
   * Create Range, with unit step.
   *
   * @param start first value of the interval
   * @param end end of interval
   */
  Range(const size_t start, const size_t end)
      : start_(start),
        end_(end),
        step_(1) {
  }

  /**
   * Create a Range with a specified step.
   *
   * @param start first value of the interval
   * @param end end of interval
   * @param step spacing between values
   */
  Range(const size_t start, const size_t end, const ptrdiff_t step) {
    // Converts an invalid definition in an empty range
    if (start > end and step > 0) {
      *this = Range();
    } else {
      start_ = start;
      end_ = end;
      step_ = step;
    }
  }

  /**
   * Check if the defined Range is defined or not
   *
   * @return true if the range is defined
   */
  bool IsEmpty() const {
    return end_ == start_;
  }

  /**
   * Compute total number of elements represented by the Range
   *
   * @return size of the range
   */
  size_t GetSize() const {
    ptrdiff_t distance = end_ - start_;
    return std::ceil(static_cast<double>(distance) / step_);
  }

  /**
   * Get nth element
   *
   * @param index index of the element
   * @return the nth element of a Range.
   */
  size_t Item(const size_t index) const {
    if (index >= GetSize())
      throw std::out_of_range("index must be < GetSize()");
    return (*this)[index];
  }

  /**
   * Get the index for this element: inverse of item
   *
   * @param item The element of the Range
   * @return the index for this item
   */
  size_t Index(const size_t item) const {
    if (not Contains(item))
      throw std::out_of_range("item not in range");
    return static_cast<ptrdiff_t>(item - start_) / step_;
  }

  /**
   * Check if the space between values is equal to one
   *
   * @return true step() == 1
   */
  bool OnlyAdjacent() const {
    return step_ == 1;
  }

  /**
   * Get the first value of the Range
   *
   * @return the first of the interval
   */
  size_t First() const {
    return start_;
  }

  /**
   * Get the last value of the Range
   *
   * @return the last of the interval
   */
  size_t Last() const {
    return IsEmpty() ? 0: end_ + (end_ > start_ ? -1 : 1);
  }

  /**
   * Is the ith element contained in this Range?
   *
   * @param item index in the original Range
   * @return true if the ith element is contained in the defined interval
   */
  bool Contains(const size_t item) const {
    if (item < std::min(First(), Last()) or item > std::max(First(), Last()))
      return false;
    return step_ == 1
        ? true
        : static_cast<ptrdiff_t>(item - start_) % step_ == 0;
  }

  /**
   * Get the start of Range
   *
   * @return start
   */
  size_t start() const {
    return start_;
  }

  /**
   * Get the end of Range
   *
   * @return end
   */
  size_t end() const {
    return end_;
  }

  /**
   * Get the step of Range
   *
   * @return step
   */
  size_t step() const {
    return step_;
  }

  /**
   * Get nth element without error check
   *
   * @param index index of the element
   * @return the nth element of a range.
   */
  size_t operator [](const size_t index) const {
    return start_ + index * step_;
  }

  /**
   * Cast the instance to an array of evenly spaced values.
   *
   * @return the array of values
   */
  operator std::vector<size_t>() const {
    std::vector<size_t> result;
    for (size_t ix = 0; ix < GetSize(); ++ix) {
      result.push_back((*this)[ix]);
    }
    return result;
  }
};

/**
 * Handle read or write of a portion or subset of a variable
 */
class Hyperslab {
 private:
  std::vector<size_t> start_;     //!< array of start for each range
  std::vector<size_t> end_;       //!< array of end for each range
  std::vector<ptrdiff_t> step_;   //!< spacing between values for each range

 public:
  /**
   * Create an empty Hyperslab: no data selection will be done
   *
   */
  Hyperslab()
      : start_(),
        end_(),
        step_() {
  }

  /**
   * Create an Hyperslab from a shape array, assumes 0 origin.
   *
   * @param shape array of lengths for each range
   */
  explicit Hyperslab(const std::vector<size_t>& shape)
      : start_(shape.size(), 0),
        end_(shape),
        step_() {
  }

  /**
   * Create an Hyperslab from a start and end interval arrays.
   *
   * @param start array of start of the interval for each range
   * @param end array of end of the interval for each range
   */
  Hyperslab(const std::vector<size_t>& start, const std::vector<size_t>& end)
      : start_(start),
        end_(end),
        step_() {
    if (start_.size() != end_.size()) {
      throw std::invalid_argument("start and end are not aligned");
    }
  }

  /**
   * Create an Hyperslab from a start, end and step interval arrays.
   *
   * @param start array of start of the interval for each range
   * @param end array of lengths of the interval for each range
   * @param step spacing between values of the interval for each range
   */
  Hyperslab(const std::vector<size_t>& start, const std::vector<size_t>& end,
            const std::vector<ptrdiff_t>& step)
      : start_(start),
        end_(end),
        step_(step) {
    if (start_.size() != end_.size()) {
      throw std::invalid_argument("start and end are not aligned");
    }
    if (start_.size() != end_.size()) {
      throw std::invalid_argument("start and step are not aligned");
    }
    for (auto &item : step) {
      if (item < 1)
        throw std::invalid_argument("stride must be > 0");
    }
  }

  /**
   * Create an Hyperslab from a start and end interval arrays.
   *
   * @param range array of range
   */
  explicit Hyperslab(const std::vector<Range>& range)
    : start_(),
      end_(),
      step_() {
    for (auto& item : range) {
      start_.push_back(item.start());
      end_.push_back(item.end());
      step_.push_back(item.step());
    }
  }

  /**
   * Get rank - number of Ranges.
   *
   * @return rank
   */
  size_t GetRank() const {
    return start_.size();
  }

  /**
   * Compute total number of elements represented by the a given Range
   * of the Hyperslab
   *
   * @param ix index into the list of ranges
   * @return ith size of the range
   */
  size_t GetSize(const size_t ix) const {
    return GetRange(ix).GetSize();
  }

  /**
   * Compute total number of elements represented by the Hyperslab
   *
   * @return total number of elements
   */
  size_t GetSize() const {
    if (IsEmpty())
      return 0;

    size_t result = 1;
    for (size_t ix = 0; ix < GetRank(); ++ix) {
      result *= GetSize(ix);
    }
    return result;
  }

  /**
   * Get the length of the interval for each Range
   *
   * @return length of the interval
   */
  const std::vector<size_t> GetSizeList() const {
    std::vector<size_t> result;
    for(size_t ix = 0; ix < GetRank(); ++ix) {
      result.push_back(GetSize(ix));
    }
    return result;
  }

  /**
   * Check if the defined Hyperslab is defined or not
   *
   * @return true if the Hyperslab is defined
   */
  bool IsEmpty() const {
    return start_.empty();
  }

  /**
   * Check if the defined Hyperslab select only adjacent values of the
   * variable.
   *
   * @return true if the Hyperslab select adjacent values
   */
  bool OnlyAdjacent() const {
    if (step_.empty())
      return true;
    return std::accumulate(step_.begin(), step_.end(), 1,
                           std::multiplies<ptrdiff_t>()) == 1;
  }

  /**
   * Get the ith Range
   *
   * @param index index into the list of Ranges
   * @return Range
   */
  Range GetRange(const int index) const {
    return step_.empty()
        ? Range(start_.at(index), end_.at(index))
        : Range(start_.at(index), end_.at(index), step_.at(index));
  }

  /**
   * Get the list of Ranges
   *
   * @return the list of Ranges
   */
  std::vector<Range> range() const {
    std::vector<Range> result;
    for (size_t ix = 0; ix < GetRank(); ++ix) {
      result.push_back(GetRange(ix));
    }
    return result;
  }

  /**
   * Compare the Hyperslab defined and the shape of the target variable
   *
   * @param shape Shape of the variable
   * @return true if the set Hyperslab selects a portion of the target
   *    variable (ie is correct)
   */
  bool operator <=(const std::vector<size_t>& shape) const {
    if (shape.size() != GetRank()) {
      return false;
    }

    for (size_t ix = 0; ix < GetRank(); ++ix) {
      if (GetSize(ix) > shape[ix])
        return false;
    }
    return true;
  }

  /**
   * Compare the Hyperslab defined and the shape of the target variable
   *
   * @param shape Shape of the variable
   * @return false if the set Hyperslab selects a larger part of the target
   *    variable (ie is incorrect)
   */
  bool operator >(const std::vector<size_t>& shape) const {
    return !(*this <= shape);
  }

  /**
   * Get the start of the interval for each range
   *
   * @return start
   */
  const std::vector<size_t>& start() const {
    return start_;
  }

  /**
   * Get the end of the interval for each range
   *
   * @return end
   */
  const std::vector<size_t>& end() const {
    return end_;
  }

  /**
   * Get the space between values of the interval for each range
   *
   * @return step
   */
  const std::vector<ptrdiff_t>& step() const {
    return step_;
  }
};

}  // namespace netcdf
