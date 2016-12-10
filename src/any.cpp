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

#include <netcdf4_cxx/any.hpp>

#include <cmath>
#include <valarray>

namespace netcdf {

using array = std::valarray<double>;

Any& Any::operator+=(const Any& rhs) {
  if (IsTyped(typeid(double)) && rhs.IsTyped(typeid(double))) {
    return this->operator+=<double>(rhs);
  }
  if (IsTyped(typeid(double)) && rhs.IsTyped(typeid(array))) {
    *this = Cast<double>() + rhs.Cast<array>();
    return *this;
  }
  if (IsTyped(typeid(array)) && rhs.IsTyped(typeid(double))) {
    *this = Cast<array>() + rhs.Cast<double>();
    return *this;
  }
  if (IsTyped(typeid(array)) && rhs.IsTyped(typeid(array))) {
    return this->operator+=<array>(rhs);
    return *this;
  }
  throw std::runtime_error("unsupported operand type(s) for +: '" + Name() +
                           "' and '" + rhs.Name() + "'");
}

Any& Any::operator-=(const Any& rhs) {
  if (IsTyped(typeid(double)) && rhs.IsTyped(typeid(double))) {
    return this->operator-=<double>(rhs);
  }
  if (IsTyped(typeid(double)) && rhs.IsTyped(typeid(array))) {
    *this = Cast<double>() - rhs.Cast<array>();
    return *this;
  }
  if (IsTyped(typeid(array)) && rhs.IsTyped(typeid(double))) {
    *this = Cast<array>() - rhs.Cast<double>();
    return *this;
  }
  if (IsTyped(typeid(array)) && rhs.IsTyped(typeid(array))) {
    return this->operator-=<array>(rhs);
    return *this;
  }
  throw std::runtime_error("unsupported operand type(s) for -: '" + Name() +
                           "' and '" + rhs.Name() + "'");
}

Any& Any::operator/=(const Any& rhs) {
  if (IsTyped(typeid(double)) && rhs.IsTyped(typeid(double))) {
    return this->operator/=<double>(rhs);
  }
  if (IsTyped(typeid(double)) && rhs.IsTyped(typeid(array))) {
    *this = Cast<double>() / rhs.Cast<array>();
    return *this;
  }
  if (IsTyped(typeid(array)) && rhs.IsTyped(typeid(double))) {
    *this = Cast<array>() / rhs.Cast<double>();
    return *this;
  }
  if (IsTyped(typeid(array)) && rhs.IsTyped(typeid(array))) {
    return this->operator/=<array>(rhs);
    return *this;
  }
  throw std::runtime_error("unsupported operand type(s) for /: '" + Name() +
                           "' and '" + rhs.Name() + "'");
}

Any& Any::operator*=(const Any& rhs) {
  if (IsTyped(typeid(double)) && rhs.IsTyped(typeid(double))) {
    return this->operator*=<double>(rhs);
  }
  if (IsTyped(typeid(double)) && rhs.IsTyped(typeid(array))) {
    *this = Cast<double>() * rhs.Cast<array>();
    return *this;
  }
  if (IsTyped(typeid(array)) && rhs.IsTyped(typeid(double))) {
    *this = Cast<array>() * rhs.Cast<double>();
    return *this;
  }
  if (IsTyped(typeid(array)) && rhs.IsTyped(typeid(array))) {
    return this->operator*=<array>(rhs);
    return *this;
  }
  throw std::runtime_error("unsupported operand type(s) for *: '" + Name() +
                           "' and '" + rhs.Name() + "'");
}

Any& Any::operator%=(const Any& rhs) {
  if (IsTyped(typeid(double)) && rhs.IsTyped(typeid(double))) {
    *this = std::fmod(Cast<double>(), rhs.Cast<double>());
    return *this;
  }
  if (IsTyped(typeid(double)) && rhs.IsTyped(typeid(array))) {
    *this = Cast<double>() % rhs.Cast<array>();
    return *this;
  }
  if (IsTyped(typeid(array)) && rhs.IsTyped(typeid(double))) {
    *this = Cast<array>() % rhs.Cast<double>();
    return *this;
  }
  if (IsTyped(typeid(array)) && rhs.IsTyped(typeid(array))) {
    *this = Cast<array>() % rhs.Cast<array>();
    ;
    return *this;
  }
  throw std::runtime_error("unsupported operand type(s) for %: '" + Name() +
                           "' and '" + rhs.Name() + "'");
}

Any operator+(const Any& lhs, const Any& rhs) {
  Any result(lhs);
  result += rhs;
  return result;
}

Any operator-(const Any& lhs, const Any& rhs) {
  Any result(lhs);
  result -= rhs;
  return result;
}

Any operator/(const Any& lhs, const Any& rhs) {
  Any result(lhs);
  result /= rhs;
  return result;
}

Any operator*(const Any& lhs, const Any& rhs) {
  Any result(lhs);
  result *= rhs;
  return result;
}

Any operator==(const Any& lhs, const Any& rhs) {
  if (lhs.IsTyped(typeid(double)) && rhs.IsTyped(typeid(double))) {
    return static_cast<double>(lhs.Cast<double>() == rhs.Cast<double>());
  }
  if (lhs.IsTyped(typeid(double)) && rhs.IsTyped(typeid(array))) {
    return lhs.Cast<double>() == rhs.Cast<array>();
  }
  if (lhs.IsTyped(typeid(array)) && rhs.IsTyped(typeid(double))) {
    return lhs.Cast<array>() == rhs.Cast<double>();
  }
  if (lhs.IsTyped(typeid(array)) && rhs.IsTyped(typeid(array))) {
    return lhs.Cast<array>() == rhs.Cast<array>();
  }
  throw std::runtime_error("unsupported operand type(s) for ==: '" +
                           lhs.Name() + "' and '" + rhs.Name() + "'");
}

Any operator!=(const Any& lhs, const Any& rhs) {
  if (lhs.IsTyped(typeid(double)) && rhs.IsTyped(typeid(double))) {
    return static_cast<double>(lhs.Cast<double>() != rhs.Cast<double>());
  }
  if (lhs.IsTyped(typeid(double)) && rhs.IsTyped(typeid(array))) {
    return lhs.Cast<double>() != rhs.Cast<array>();
  }
  if (lhs.IsTyped(typeid(array)) && rhs.IsTyped(typeid(double))) {
    return lhs.Cast<array>() != rhs.Cast<double>();
  }
  if (lhs.IsTyped(typeid(array)) && rhs.IsTyped(typeid(array))) {
    return lhs.Cast<array>() != rhs.Cast<array>();
  }
  throw std::runtime_error("unsupported operand type(s) for !=: '" +
                           lhs.Name() + "' and '" + rhs.Name() + "'");
}

Any operator&&(const Any& lhs, const Any& rhs) {
  if (lhs.IsTyped(typeid(double)) && rhs.IsTyped(typeid(double))) {
    return static_cast<double>(lhs.Cast<double>() && rhs.Cast<double>());
  }
  if (lhs.IsTyped(typeid(double)) && rhs.IsTyped(typeid(array))) {
    return lhs.Cast<double>() && rhs.Cast<array>();
  }
  if (lhs.IsTyped(typeid(array)) && rhs.IsTyped(typeid(double))) {
    return lhs.Cast<array>() && rhs.Cast<double>();
  }
  if (lhs.IsTyped(typeid(array)) && rhs.IsTyped(typeid(array))) {
    return lhs.Cast<array>() && rhs.Cast<array>();
  }
  throw std::runtime_error("unsupported operand type(s) for &&: '" +
                           lhs.Name() + "' and '" + rhs.Name() + "'");
}

Any operator||(const Any& lhs, const Any& rhs) {
  if (lhs.IsTyped(typeid(double)) && rhs.IsTyped(typeid(double))) {
    return static_cast<double>(lhs.Cast<double>() || rhs.Cast<double>());
  }
  if (lhs.IsTyped(typeid(double)) && rhs.IsTyped(typeid(array))) {
    return lhs.Cast<double>() || rhs.Cast<array>();
  }
  if (lhs.IsTyped(typeid(array)) && rhs.IsTyped(typeid(double))) {
    return lhs.Cast<array>() || rhs.Cast<double>();
  }
  if (lhs.IsTyped(typeid(array)) && rhs.IsTyped(typeid(array))) {
    return lhs.Cast<array>() || rhs.Cast<array>();
  }
  throw std::runtime_error("unsupported operand type(s) for ||: '" +
                           lhs.Name() + "' and '" + rhs.Name() + "'");
}

Any operator>(const Any& lhs, const Any& rhs) {
  if (lhs.IsTyped(typeid(double)) && rhs.IsTyped(typeid(double))) {
    return static_cast<double>(lhs.Cast<double>() > rhs.Cast<double>());
  }
  if (lhs.IsTyped(typeid(double)) && rhs.IsTyped(typeid(array))) {
    return lhs.Cast<double>() > rhs.Cast<array>();
  }
  if (lhs.IsTyped(typeid(array)) && rhs.IsTyped(typeid(double))) {
    return lhs.Cast<array>() > rhs.Cast<double>();
  }
  if (lhs.IsTyped(typeid(array)) && rhs.IsTyped(typeid(array))) {
    return lhs.Cast<array>() > rhs.Cast<array>();
  }
  throw std::runtime_error("unsupported operand type(s) for >: '" + lhs.Name() +
                           "' and '" + rhs.Name() + "'");
}

Any operator>=(const Any& lhs, const Any& rhs) {
  if (lhs.IsTyped(typeid(double)) && rhs.IsTyped(typeid(double))) {
    return static_cast<double>(lhs.Cast<double>() >= rhs.Cast<double>());
  }
  if (lhs.IsTyped(typeid(double)) && rhs.IsTyped(typeid(array))) {
    return lhs.Cast<double>() >= rhs.Cast<array>();
  }
  if (lhs.IsTyped(typeid(array)) && rhs.IsTyped(typeid(double))) {
    return lhs.Cast<array>() >= rhs.Cast<double>();
  }
  if (lhs.IsTyped(typeid(array)) && rhs.IsTyped(typeid(array))) {
    return lhs.Cast<array>() >= rhs.Cast<array>();
  }
  throw std::runtime_error("unsupported operand type(s) for >=: '" +
                           lhs.Name() + "' and '" + rhs.Name() + "'");
}

Any operator<(const Any& lhs, const Any& rhs) {
  if (lhs.IsTyped(typeid(double)) && rhs.IsTyped(typeid(double))) {
    return static_cast<double>(lhs.Cast<double>() < rhs.Cast<double>());
  }
  if (lhs.IsTyped(typeid(double)) && rhs.IsTyped(typeid(array))) {
    return lhs.Cast<double>() < rhs.Cast<array>();
  }
  if (lhs.IsTyped(typeid(array)) && rhs.IsTyped(typeid(double))) {
    return lhs.Cast<array>() < rhs.Cast<double>();
  }
  if (lhs.IsTyped(typeid(array)) && rhs.IsTyped(typeid(array))) {
    return lhs.Cast<array>() < rhs.Cast<array>();
  }
  throw std::runtime_error("unsupported operand type(s) for <: '" + lhs.Name() +
                           "' and '" + rhs.Name() + "'");
}

Any operator<=(const Any& lhs, const Any& rhs) {
  if (lhs.IsTyped(typeid(double)) && rhs.IsTyped(typeid(double))) {
    return static_cast<double>(lhs.Cast<double>() <= rhs.Cast<double>());
  }
  if (lhs.IsTyped(typeid(double)) && rhs.IsTyped(typeid(array))) {
    return lhs.Cast<double>() <= rhs.Cast<array>();
  }
  if (lhs.IsTyped(typeid(array)) && rhs.IsTyped(typeid(double))) {
    return lhs.Cast<array>() <= rhs.Cast<double>();
  }
  if (lhs.IsTyped(typeid(array)) && rhs.IsTyped(typeid(array))) {
    return lhs.Cast<array>() <= rhs.Cast<array>();
  }
  throw std::runtime_error("unsupported operand type(s) for >: '" + lhs.Name() +
                           "' and '" + rhs.Name() + "'");
}

Any Any::abs(const Any& x) {
  if (x.IsTyped(typeid(double))) {
    return std::abs(x.Cast<double>());
  }
  if (x.IsTyped(typeid(array))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::abs(x.Cast<array>()));
  }
  throw std::runtime_error("unsupported operand type for abs: '" + x.Name() +
                           "'");
}

Any Any::exp(const Any& x) {
  if (x.IsTyped(typeid(double))) {
    return std::exp(x.Cast<double>());
  }
  if (x.IsTyped(typeid(array))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::exp(x.Cast<array>()));
  }
  throw std::runtime_error("unsupported operand type for exp: '" + x.Name() +
                           "'");
}

Any Any::log(const Any& x) {
  if (x.IsTyped(typeid(double))) {
    return std::log(x.Cast<double>());
  }
  if (x.IsTyped(typeid(array))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::log(x.Cast<array>()));
  }
  throw std::runtime_error("unsupported operand type for log: '" + x.Name() +
                           "'");
}

Any Any::log10(const Any& x) {
  if (x.IsTyped(typeid(double))) {
    return std::log10(x.Cast<double>());
  }
  if (x.IsTyped(typeid(array))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::log10(x.Cast<array>()));
  }
  throw std::runtime_error("unsupported operand type for log10: '" + x.Name() +
                           "'");
}

Any Any::sqrt(const Any& x) {
  if (x.IsTyped(typeid(double))) {
    return std::sqrt(x.Cast<double>());
  }
  if (x.IsTyped(typeid(array))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::sqrt(x.Cast<array>()));
  }
  throw std::runtime_error("unsupported operand type for sqrt: '" + x.Name() +
                           "'");
}

Any Any::sin(const Any& x) {
  if (x.IsTyped(typeid(double))) {
    return std::sin(x.Cast<double>());
  }
  if (x.IsTyped(typeid(array))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::sin(x.Cast<array>()));
  }
  throw std::runtime_error("unsupported operand type for sin: '" + x.Name() +
                           "'");
}

Any Any::cos(const Any& x) {
  if (x.IsTyped(typeid(double))) {
    return std::cos(x.Cast<double>());
  }
  if (x.IsTyped(typeid(array))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::cos(x.Cast<array>()));
  }
  throw std::runtime_error("unsupported operand type for cos: '" + x.Name() +
                           "'");
}

Any Any::tan(const Any& x) {
  if (x.IsTyped(typeid(double))) {
    return std::tan(x.Cast<double>());
  }
  if (x.IsTyped(typeid(array))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::tan(x.Cast<array>()));
  }
  throw std::runtime_error("unsupported operand type for tan: '" + x.Name() +
                           "'");
}

Any Any::asin(const Any& x) {
  if (x.IsTyped(typeid(double))) {
    return std::asin(x.Cast<double>());
  }
  if (x.IsTyped(typeid(array))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::asin(x.Cast<array>()));
  }
  throw std::runtime_error("unsupported operand type for asin: '" + x.Name() +
                           "'");
}

Any Any::acos(const Any& x) {
  if (x.IsTyped(typeid(double))) {
    return std::acos(x.Cast<double>());
  }
  if (x.IsTyped(typeid(array))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::acos(x.Cast<array>()));
  }
  throw std::runtime_error("unsupported operand type for acos: '" + x.Name() +
                           "'");
}

Any Any::atan(const Any& x) {
  if (x.IsTyped(typeid(double))) {
    return std::atan(x.Cast<double>());
  }
  if (x.IsTyped(typeid(array))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::atan(x.Cast<array>()));
  }
  throw std::runtime_error("unsupported operand type for atan: '" + x.Name() +
                           "'");
}

Any Any::sinh(const Any& x) {
  if (x.IsTyped(typeid(double))) {
    return std::sinh(x.Cast<double>());
  }
  if (x.IsTyped(typeid(array))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::sinh(x.Cast<array>()));
  }
  throw std::runtime_error("unsupported operand type for sinh: '" + x.Name() +
                           "'");
}

Any Any::cosh(const Any& x) {
  if (x.IsTyped(typeid(double))) {
    return std::cosh(x.Cast<double>());
  }
  if (x.IsTyped(typeid(array))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::cosh(x.Cast<array>()));
  }
  throw std::runtime_error("unsupported operand type for cosh: '" + x.Name() +
                           "'");
}

Any Any::tanh(const Any& x) {
  if (x.IsTyped(typeid(double))) {
    return std::tanh(x.Cast<double>());
  }
  if (x.IsTyped(typeid(array))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::tanh(x.Cast<array>()));
  }
  throw std::runtime_error("unsupported operand type for tanh: '" + x.Name() +
                           "'");
}

Any Any::pow(const Any& x, const Any& y) {
  if (x.IsTyped(typeid(double)) && y.IsTyped(typeid(double))) {
    return std::pow(x.Cast<double>(), y.Cast<double>());
  }
  if (x.IsTyped(typeid(double)) && y.IsTyped(typeid(array))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::pow(x.Cast<double>(), y.Cast<array>()));
  }
  if (x.IsTyped(typeid(array)) && y.IsTyped(typeid(double))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::pow(x.Cast<array>(), y.Cast<double>()));
  }
  if (x.IsTyped(typeid(array)) && y.IsTyped(typeid(array))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::pow(x.Cast<array>(), y.Cast<array>()));
  }
  throw std::runtime_error("unsupported operand type(s) for pow: '" + x.Name() +
                           "' and '" + y.Name() + "'");
}

Any Any::atan2(const Any& x, const Any& y) {
  if (x.IsTyped(typeid(double)) && y.IsTyped(typeid(double))) {
    return std::atan2(x.Cast<double>(), y.Cast<double>());
  }
  if (x.IsTyped(typeid(double)) && y.IsTyped(typeid(array))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::atan2(x.Cast<double>(), y.Cast<array>()));
  }
  if (x.IsTyped(typeid(array)) && y.IsTyped(typeid(double))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::atan2(x.Cast<array>(), y.Cast<double>()));
  }
  if (x.IsTyped(typeid(array)) && y.IsTyped(typeid(array))) {
    // invisible proxy type cause "auto" to deduce the "wrong" type
    return static_cast<array>(std::atan2(x.Cast<array>(), y.Cast<array>()));
  }
  throw std::runtime_error("unsupported operand type(s) for atan2: '" +
                           x.Name() + "' and '" + y.Name() + "'");
}

const Any& Any::iif(const Any& condition, const Any& if_true,
                    const Any& if_false) {
  double scalar = condition;
  return scalar != 0 ? if_true : if_false;
}
}  // namespace netcdf