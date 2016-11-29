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
#include <cstring>
#include <netcdf4_cxx/attribute.hpp>
#include <netcdf4_cxx/type.hpp>
#include <string>
#include <vector>

#include "tempfile.hpp"

BOOST_AUTO_TEST_SUITE(test_attribute)

BOOST_AUTO_TEST_CASE(test_string) {
  Object object;
  netcdf::Attribute att(object, "attribute");
  std::vector<std::string> strings = {
      "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean ",
      "ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et ",
      "dis parturient montes, nascetur ridiculus mus. Donec quam felis, ",
      "ultricies nec, pellentesque eu, pretium quis, "};
  att.WriteString(strings);

  BOOST_CHECK(att.GetDataType().GetPrimitive() ==
              netcdf::type::Primitive::kString);
  BOOST_CHECK_EQUAL(att.IsString(), true);
  BOOST_CHECK_EQUAL(att.IsText(), false);
  BOOST_CHECK_EQUAL(att.name(), "attribute");
  BOOST_CHECK_EQUAL(att.GetLength(), 4);

  int idx = 0;
  for (auto& item : att.ReadString()) {
    BOOST_CHECK_EQUAL(item, strings[idx++]);
  }
}

BOOST_AUTO_TEST_CASE(test_text) {
  Object object;
  std::string text("Lorem ipsum dolor sit amet");

  netcdf::Attribute att(object, "attribute");
  att = netcdf::Attribute(object, "attribute");
  att.WriteText(text);

  BOOST_CHECK(att.GetDataType().GetPrimitive() ==
              netcdf::type::Primitive::kChar);
  BOOST_CHECK_EQUAL(att.IsString(), false);
  BOOST_CHECK_EQUAL(att.IsText(), true);
  BOOST_CHECK_EQUAL(att.GetLength(), text.size());
  BOOST_CHECK_EQUAL(att.name(), "attribute");
  BOOST_CHECK_EQUAL(att.ReadText(), text);
}

#define TEST_ATT(NAME, NC_TYPE, CPP_TYPE)         \
  BOOST_AUTO_TEST_CASE(NAME) {                    \
    Object object;                                \
    std::vector<CPP_TYPE> in, out;                \
    for (int i = 0; i < 10; ++i) {                \
      in.push_back(i);                            \
    }                                             \
    netcdf::Attribute att(object, "attribute");   \
    att.Write(netcdf::type::NC_TYPE(object), in); \
    BOOST_CHECK_EQUAL(att.IsString(), false);     \
    BOOST_CHECK_EQUAL(att.IsText(), false);       \
    BOOST_CHECK_EQUAL(att.name(), "attribute");   \
    BOOST_CHECK_EQUAL(att.GetLength(), 10);       \
    out = att.Read<CPP_TYPE>();                   \
    for (int i = 0; i < 10; ++i) {                \
      BOOST_CHECK_EQUAL(out.at(i), in.at(i));     \
    }                                             \
  }

TEST_ATT(test_byte, Byte, signed char)
TEST_ATT(test_short, Short, short)
TEST_ATT(test_int, Int, int)
TEST_ATT(test_int64, Int64, long long)
TEST_ATT(test_ubyte, UnsignedByte, unsigned char)
TEST_ATT(test_uint, UnsignedInt, unsigned int)
TEST_ATT(test_uint64, UnsignedInt64, unsigned long long)
TEST_ATT(test_float, Float, float)
TEST_ATT(test_double, Double, double)

BOOST_AUTO_TEST_CASE(test_compound) {
  struct Compound {
    short a;
    short b;
  };
  Object object;
  netcdf::type::Compound type(object, "MyCompound", sizeof(Compound));
  type.InsertMember("a", NC_COMPOUND_OFFSET(Compound, a),
                    netcdf::type::Short(object));
  type.InsertMember("b", NC_COMPOUND_OFFSET(Compound, b),
                    netcdf::type::Short(object));
  netcdf::Attribute att(object, "attribute");
  std::vector<Compound> in({{12, 24}, {48, 96}}), out;

  att.Write(type, in);

  int idx = 0;
  for (auto& item : att.Read<Compound>()) {
    BOOST_CHECK_EQUAL(item.a, in[idx].a);
    BOOST_CHECK_EQUAL(item.b, in[idx++].b);
  }
  BOOST_CHECK(att.GetDataType().GetPrimitive() ==
              netcdf::type::Primitive::kCompound);
  BOOST_CHECK_EQUAL(att.IsString(), false);
  BOOST_CHECK_EQUAL(att.IsText(), false);
  BOOST_CHECK_EQUAL(att.GetLength(), in.size());
  BOOST_CHECK_EQUAL(att.name(), "attribute");
}

BOOST_AUTO_TEST_SUITE_END()
