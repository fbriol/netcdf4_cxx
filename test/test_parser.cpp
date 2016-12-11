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

#include <netcdf.h>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <netcdf4_cxx/file.hpp>
#include <netcdf4_cxx/parser.hpp>
#include <netcdf4_cxx/parser.hpp>
#include <netcdf4_cxx/query.hpp>
#include <string>

class QueryProxy : public netcdf::QueryProxy {
 public:
  QueryProxy() : netcdf::QueryProxy(netcdf::Query(), netcdf::File(), "") {}

  double Evaluate(const std::string& query) const {
    return netcdf::parser::LiteralExpression(*this, query).Evaluate();
  }
};

BOOST_AUTO_TEST_SUITE(test_parser)

BOOST_AUTO_TEST_CASE(test_number) {
  QueryProxy query;

  BOOST_CHECK_EQUAL(query.Evaluate("42"), 42);
  BOOST_CHECK_EQUAL(query.Evaluate("-2"), -2);
  BOOST_CHECK_EQUAL(query.Evaluate("+2"), 2);
  BOOST_CHECK_EQUAL(query.Evaluate("+4."), 4);
}

BOOST_AUTO_TEST_CASE(test_constant) {
  QueryProxy query;

  BOOST_CHECK_EQUAL(query.Evaluate("e"), M_E);
  BOOST_CHECK_EQUAL(query.Evaluate("log2e"), M_LOG2E);
  BOOST_CHECK_EQUAL(query.Evaluate("log10e"), M_LOG10E);
  BOOST_CHECK_EQUAL(query.Evaluate("ln2"), M_LN2);
  BOOST_CHECK_EQUAL(query.Evaluate("ln10"), M_LN10);
  BOOST_CHECK_EQUAL(query.Evaluate("pi"), M_PI);
  BOOST_CHECK_EQUAL(query.Evaluate("pi_2"), M_PI_2);
  BOOST_CHECK_EQUAL(query.Evaluate("pi_4"), M_PI_4);
  BOOST_CHECK_EQUAL(query.Evaluate("1_pi"), M_1_PI);
  BOOST_CHECK_EQUAL(query.Evaluate("2_pi"), M_2_PI);
  BOOST_CHECK_EQUAL(query.Evaluate("2_sqrtpi"), M_2_SQRTPI);
  BOOST_CHECK_EQUAL(query.Evaluate("sqrt2"), M_SQRT2);
}

BOOST_AUTO_TEST_CASE(test_variable) {
  QueryProxy query;

  BOOST_CHECK_EQUAL(query.Evaluate("x=1; 1/x"), 1);
  BOOST_CHECK_EQUAL(query.Evaluate("a_01=1; 1/a_01"), 1);
  BOOST_CHECK_EQUAL(query.Evaluate("a=1; aa=2; aaa=3; a+2*aa+3*(aaa*aaa)"), 32);
  BOOST_CHECK_EQUAL(query.Evaluate("a=1; A=2; a+A"), 3);
}

BOOST_AUTO_TEST_CASE(test_function) {
  QueryProxy query;

  BOOST_CHECK_EQUAL(query.Evaluate("sin(pi_2)"), std::sin(M_PI_2));
  BOOST_CHECK_EQUAL(query.Evaluate("cos(pi_2)"), std::cos(M_PI_2));
  BOOST_CHECK_EQUAL(query.Evaluate("tan(pi_2)"), std::tan(M_PI_2));

  BOOST_CHECK_EQUAL(query.Evaluate("asin(pi_4)"), std::asin(M_PI_4));
  BOOST_CHECK_EQUAL(query.Evaluate("acos(pi_4)"), std::acos(M_PI_4));
  BOOST_CHECK_EQUAL(query.Evaluate("atan(pi_4)"), std::atan(M_PI_4));

  BOOST_CHECK_EQUAL(query.Evaluate("sinh(pi_2)"), std::sinh(M_PI_2));
  BOOST_CHECK_EQUAL(query.Evaluate("cosh(pi_2)"), std::cosh(M_PI_2));
  BOOST_CHECK_EQUAL(query.Evaluate("tanh(pi_2)"), std::tanh(M_PI_2));

  BOOST_CHECK_EQUAL(query.Evaluate("atan2(pi_4, pi_2)"),
                    std::atan2(M_PI_4, M_PI_2));

  BOOST_CHECK_EQUAL(query.Evaluate("abs(-pi_2)"), M_PI_2);
  BOOST_CHECK_EQUAL(query.Evaluate("exp(1)"), M_E);
  BOOST_CHECK_EQUAL(query.Evaluate("log(1)"), 0);
  BOOST_CHECK_EQUAL(query.Evaluate("log10(0.1)"), -1);
  BOOST_CHECK_EQUAL(query.Evaluate("sqrt(16)"), 4);
  BOOST_CHECK_EQUAL(query.Evaluate("pow(4, 4)"), 256);

  BOOST_CHECK_EQUAL(query.Evaluate("iif(1, 2, 3)"), 2);
  BOOST_CHECK_EQUAL(query.Evaluate("iif(0, 2, 3)"), 3);
}

BOOST_AUTO_TEST_CASE(test_comparison) {
  QueryProxy query;

  BOOST_CHECK_EQUAL(query.Evaluate("1 > 1"), 0);
  BOOST_CHECK_EQUAL(query.Evaluate("1 >= 1"), 1);
  BOOST_CHECK_EQUAL(query.Evaluate("1 < 1"), 0);
  BOOST_CHECK_EQUAL(query.Evaluate("1 <= 1"), 1);

  BOOST_CHECK_EQUAL(query.Evaluate("1 == 1"), 1);
  BOOST_CHECK_EQUAL(query.Evaluate("1 != 1"), 0);
}

BOOST_AUTO_TEST_CASE(test_logic) {
  QueryProxy query;

  BOOST_CHECK_EQUAL(query.Evaluate("1 && 1"), 1);
  BOOST_CHECK_EQUAL(query.Evaluate("1 && 0"), 0);
  BOOST_CHECK_EQUAL(query.Evaluate("0 && 1"), 0);
  BOOST_CHECK_EQUAL(query.Evaluate("0 && 0"), 0);
  BOOST_CHECK_EQUAL(query.Evaluate("1 || 1"), 1);
  BOOST_CHECK_EQUAL(query.Evaluate("1 || 0"), 1);
  BOOST_CHECK_EQUAL(query.Evaluate("0 || 1"), 1);
  BOOST_CHECK_EQUAL(query.Evaluate("0 || 0"), 0);
  //   BOOST_CHECK_EQUAL(query.Evaluate("2 & 6"), 2);
  //   BOOST_CHECK_EQUAL(query.Evaluate("2 | 1"), 3);
}

BOOST_AUTO_TEST_CASE(test_operator) {
  QueryProxy query;

  BOOST_CHECK_EQUAL(query.Evaluate("2 + 2"), 4);
  BOOST_CHECK_EQUAL(query.Evaluate("2 - 2"), 0);
  BOOST_CHECK_EQUAL(query.Evaluate("2 * 3"), 6);
  BOOST_CHECK_EQUAL(query.Evaluate("2 / 3"), 2.0 / 3.0);
  BOOST_CHECK_EQUAL(query.Evaluate("3 % 2"), 1);
}

BOOST_AUTO_TEST_CASE(test_basic) {
  QueryProxy query;

  BOOST_CHECK_EQUAL(query.Evaluate("pow(1, 3) - 4 * 1 + 4"), 1);
  BOOST_CHECK_EQUAL(query.Evaluate("pow(sin(0.7), 3)"),
                    std::pow(std::sin(0.7), 3));
  BOOST_CHECK_EQUAL(query.Evaluate("11.2 * sin(0.41) + 0.6 * tan(-0.66)"),
                    11.2 * std::sin(0.41) + 0.6 * std::tan(-0.66));
  double x = M_PI_4;
  BOOST_CHECK_EQUAL(
      query.Evaluate("x=pi_4; ((((1/(pow(cos(x), "
                     "2)))*cos(x))-(tan(x)*(-sin(x))))/(pow(cos(x),2)))"),
      ((((1 / (std::pow(std::cos(x), 2))) * std::cos(x)) -
        (std::tan(x) * (-std::sin(x)))) /
       (std::pow(std::cos(x), 2))));
}

BOOST_AUTO_TEST_SUITE_END()
