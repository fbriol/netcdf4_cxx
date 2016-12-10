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
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeinfo>

namespace netcdf {

template <class T>
using StorageType = typename std::decay<T>::type;

/**
 * Defines a type of object to be thrown by the value-returning forms of
 * Any::Cast on failure.
 */
class BadAnyCast : public std::bad_cast {
 private:
  std::string msg_;

 public:
  /**
   * Constructs the exception object with msg as explanatory string
   *
   * @param msg explanatory string
   */
  BadAnyCast(const std::string& msg) : std::bad_cast() { msg_ = msg; }

  /**
   * Returns an explanatory string
   *
   * @return Pointer to a null-terminated string with explanatory information
   */
  virtual const char* what() const noexcept override { return msg_.c_str(); }
};

/**
 * Type-safe container for single values of any type.
 */
class Any {
 public:
  /**
   * Default constructor
   */
  constexpr Any() noexcept : ptr_(nullptr) {}

  /**
   * Copy constructor
   *
   * @param rhs right hand side
   */
  Any(const Any& rhs) : ptr_(rhs.Clone()) {}

  /**
   * Move constructor
   *
   * @param rhs right hand side
   */
  constexpr Any(Any&& rhs) noexcept : ptr_(rhs.ptr_) { rhs.ptr_ = nullptr; }

  /***
   * Test if this instance is defined
   *
   * @return true if this instance is defined
   */
  bool IsEmpty() const noexcept { return ptr_ == nullptr; }

  /**
   * Constructs an object of type Any that contains an object of type T
   * direct-initialized with std::forward<T>(value).
   *
   * @param value   value to initialize the contained value with
   */
  template <typename T, typename = typename std::enable_if<
                            !std::is_same<StorageType<T>, Any>::value>::type>
  Any(T&& value) : ptr_(nullptr) {
    static_assert(std::is_copy_constructible<StorageType<T>>::value,
                  "template must be copy-constructible");
    ptr_ = new Derived<StorageType<T>>(std::forward<T>(value));
  }

  /**
   * Checks if this instance contains a value of type "type"
   *
   * @param type type to test
   * @return true if this instance contains a value of type "type"
   */
  bool IsTyped(const std::type_info& type) const {
    return ptr_->Type() == type;
  }

  /**
   * Cast this instance as T, throws BadAnyCast on failure
   *
   * @return the contained object
   */
  template <class T>
  T& Cast() {
    auto derived = dynamic_cast<Derived<StorageType<T>>*>(ptr_);
    if (derived == nullptr)
      throw BadAnyCast(Name() + "not a " + typeid(T).name());

    return derived->value_;
  }

  /**
   * Cast this instance as T, throws BadAnyCast on failure
   *
   * @return the contained object
   */
  template <class T>
  const T& Cast() const {
    auto derived = dynamic_cast<Derived<StorageType<T>>*>(ptr_);
    if (derived == nullptr)
      throw BadAnyCast(Name() + "not a " + typeid(T).name());

    return derived->value_;
  }

  /**
   * Cast this instance as T, throws BadAnyCast on failure
   *
   * @return the contained object
   */
  template <class T>
  constexpr operator T() {
    return Cast<StorageType<T>>();
  }

  /**
   * Cast this instance as const T, throws BadAnyCast on failure
   *
   * @return the contained object
   */
  template <class T>
  constexpr operator const T() const {
    return Cast<StorageType<const T>>();
  }

  /**
   * Assigns contents to the contained value
   *
   * @param rhs object whose contained value to assign
   * @return *this
   */
  Any& operator=(const Any& rhs) {
    if (ptr_ == rhs.ptr_) return *this;

    auto ptr = ptr_;
    ptr_ = rhs.Clone();
    delete ptr;

    return *this;
  }

  /**
   * Move operator
   *
   * @param rhs value to initialize the contained value with
   * @return *this
   */
  Any& operator=(Any&& rhs) {
    if (ptr_ == rhs.ptr_) return *this;

    std::swap(ptr_, rhs.ptr_);

    return *this;
  }

  /**
   * Deallocates this instance
   */
  ~Any() { delete ptr_; }

  /**
   * Gets the type info of the value stored in this instance
   *
   * @return type info of the value contained
   */
  const std::type_info& Type() const noexcept { return ptr_->Type(); }

  /**
   * Gets the name of the type of the value stored in this instance
   *
   * @return the name of the value contained
   */
  std::string Name() const { return ptr_->Type().name(); }

  // Operators
  template <typename T>
  Any& operator+=(const Any& rhs) {
    Cast<T>() += rhs.Cast<T>();
    return *this;
  }

  template <typename T>
  Any& operator-=(const Any& rhs) {
    Cast<T>() -= rhs.Cast<T>();
    return *this;
  }

  template <typename T>
  Any& operator/=(const Any& rhs) {
    Cast<T>() /= rhs.Cast<T>();
    return *this;
  }

  template <typename T>
  Any& operator*=(const Any& rhs) {
    Cast<T>() *= rhs.Cast<T>();
    return *this;
  }

  template <typename T>
  Any& operator%=(const Any& rhs) {
    Cast<T>() %= rhs.Cast<T>();
    return *this;
  }

  static Any abs(const Any& x);

  // Exponential functions
  static Any exp(const Any& x);
  static Any log(const Any& x);
  static Any log10(const Any& x);

  // Power functions
  static Any pow(const Any& x, const Any& y);
  static Any sqrt(const Any& x);

  // Trigonometric functions
  static Any sin(const Any& x);
  static Any cos(const Any& x);
  static Any tan(const Any& x);
  static Any asin(const Any& x);
  static Any acos(const Any& x);
  static Any atan(const Any& x);
  static Any atan2(const Any& y, const Any& x);

  // Hyperbolic functions
  static Any sinh(const Any& x);
  static Any cosh(const Any& x);
  static Any tanh(const Any& x);

  Any& operator+=(const Any& rhs);
  Any& operator-=(const Any& rhs);
  Any& operator/=(const Any& rhs);
  Any& operator*=(const Any& rhs);
  Any& operator%=(const Any& rhs);

  inline Any operator+(const Any& rhs) const {
    Any lhs(*this);
    lhs += rhs;
    return lhs;
  }

  inline Any operator-(const Any& rhs) const {
    Any lhs(*this);
    lhs -= rhs;
    return lhs;
  }

  inline Any operator/(const Any& rhs) const {
    Any lhs(*this);
    lhs /= rhs;
    return lhs;
  }

  inline Any operator*(const Any& rhs) const {
    Any lhs(*this);
    lhs *= rhs;
    return lhs;
  }

  inline Any operator%(const Any& rhs) const {
    Any lhs(*this);
    lhs %= rhs;
    return lhs;
  }

  inline Any operator-() const {
    Any value = -1.0;
    return this->operator*(value);
  }

  inline Any& operator-() {
    Any value = -1.0;
    *this *= value;
    return *this;
  }

  inline Any operator+() const { return Any(*this); }

  inline Any& operator+() { return *this; }

  friend Any operator+(const Any& lhs, const Any& rhs);
  friend Any operator-(const Any& lhs, const Any& rhs);
  friend Any operator/(const Any& lhs, const Any& rhs);
  friend Any operator*(const Any& lhs, const Any& rhs);
  friend Any operator%(const Any& lhs, const Any& rhs);

  friend Any operator&&(const Any& lhs, const Any& rhs);
  friend Any operator||(const Any& lhs, const Any& rhs);

  friend Any operator==(const Any& lhs, const Any& rhs);
  friend Any operator!=(const Any& lhs, const Any& rhs);
  friend Any operator<(const Any& lhs, const Any& rhs);
  friend Any operator<=(const Any& lhs, const Any& rhs);
  friend Any operator>(const Any& lhs, const Any& rhs);
  friend Any operator>=(const Any& lhs, const Any& rhs);

  static const Any& iif(const Any& condition, const Any& if_true,
                        const Any& if_false);

 private:
  // Base object which handles the value stored in this instance
  struct Base {
    virtual ~Base() noexcept {}

    virtual Base* Clone() const = 0;

    virtual const std::type_info& Type() noexcept { return typeid(void); }
  };

  // Real object which handles the value stored in this instance
  template <typename T>
  struct Derived : Base {
    T value_;

    template <typename U>
    constexpr Derived(U&& value) : value_(std::forward<U>(value)) {}

    Base* Clone() const override { return new Derived<T>(value_); }

    const std::type_info& Type() noexcept override { return typeid(T); }
  };

  // Clone the value stored in this instance
  Base* Clone() const { return ptr_ ? ptr_->Clone() : nullptr; }

  // Pointer to the value stored
  Base* ptr_;
};
}  // namespace netcdf