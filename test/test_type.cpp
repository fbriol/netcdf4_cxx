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

#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <netcdf/object.hpp>
#include <netcdf/type.hpp>
#include <netcdf.h>
#include <string>
#include <utility>
#include <vector>

#include "tempfile.hpp"

BOOST_AUTO_TEST_SUITE (test_type)

// netcdf::type::Byte
BOOST_AUTO_TEST_CASE( type_byte ) {
  Object object;
  netcdf::type::Byte type(object);
  BOOST_CHECK_NE(type.nc_id(), 0);
  BOOST_CHECK_EQUAL(type.id(), netcdf::type::kByte);
  BOOST_CHECK_EQUAL(type.GetSize(), sizeof(signed char));
  BOOST_CHECK_EQUAL(type.IsFloatingPoint(), false);
  BOOST_CHECK_EQUAL(type.IsIntegral(), true);
  BOOST_CHECK_EQUAL(type.IsNumeric(), true);
  BOOST_CHECK_EQUAL(type.IsPrimitive(), true);
  BOOST_CHECK_EQUAL(type.IsString(), false);
  BOOST_CHECK_EQUAL(type.IsUserType(), false);
  BOOST_CHECK_EQUAL(type.IsEnum(), false);
  BOOST_CHECK_EQUAL(type.IsOpaque(), false);
  BOOST_CHECK_EQUAL(type.IsVLen(), false);
  BOOST_CHECK_EQUAL(type.IsCompound(), false);
  BOOST_CHECK_EQUAL(type.GetPrimitiveName(), "byte");
}

// netcdf::type::Char
BOOST_AUTO_TEST_CASE( type_char ) {
  Object object;
  netcdf::type::Char type(object);
  BOOST_CHECK_NE(type.nc_id(), 0);
  BOOST_CHECK_EQUAL(type.id(), netcdf::type::kChar);
  BOOST_CHECK_EQUAL(type.GetSize(), sizeof(char));
  BOOST_CHECK_EQUAL(type.IsFloatingPoint(), false);
  BOOST_CHECK_EQUAL(type.IsIntegral(), false);
  BOOST_CHECK_EQUAL(type.IsNumeric(), false);
  BOOST_CHECK_EQUAL(type.IsPrimitive(), true);
  BOOST_CHECK_EQUAL(type.IsString(), true);
  BOOST_CHECK_EQUAL(type.IsUserType(), false);
  BOOST_CHECK_EQUAL(type.IsEnum(), false);
  BOOST_CHECK_EQUAL(type.IsOpaque(), false);
  BOOST_CHECK_EQUAL(type.IsVLen(), false);
  BOOST_CHECK_EQUAL(type.IsCompound(), false);
  BOOST_CHECK_EQUAL(type.GetPrimitiveName(), "char");
}

// netcdf::type::Double
BOOST_AUTO_TEST_CASE( type_double ) {
  Object object;
  netcdf::type::Double type(object);
  BOOST_CHECK_NE(type.nc_id(), 0);
  BOOST_CHECK_EQUAL(type.id(), netcdf::type::kDouble);
  BOOST_CHECK_EQUAL(type.GetSize(), sizeof(double));
  BOOST_CHECK_EQUAL(type.IsFloatingPoint(), true);
  BOOST_CHECK_EQUAL(type.IsIntegral(), false);
  BOOST_CHECK_EQUAL(type.IsNumeric(), true);
  BOOST_CHECK_EQUAL(type.IsPrimitive(), true);
  BOOST_CHECK_EQUAL(type.IsString(), false);
  BOOST_CHECK_EQUAL(type.IsUserType(), false);
  BOOST_CHECK_EQUAL(type.IsEnum(), false);
  BOOST_CHECK_EQUAL(type.IsOpaque(), false);
  BOOST_CHECK_EQUAL(type.IsVLen(), false);
  BOOST_CHECK_EQUAL(type.IsCompound(), false);
  BOOST_CHECK_EQUAL(type.GetPrimitiveName(), "double");
}

// netcdf::type::Float
BOOST_AUTO_TEST_CASE( type_float) {
  Object object;
  netcdf::type::Float type(object);
  BOOST_CHECK_NE(type.nc_id(), 0);
  BOOST_CHECK_EQUAL(type.id(), netcdf::type::kFloat);
  BOOST_CHECK_EQUAL(type.GetSize(), sizeof(float));
  BOOST_CHECK_EQUAL(type.IsFloatingPoint(), true);
  BOOST_CHECK_EQUAL(type.IsIntegral(), false);
  BOOST_CHECK_EQUAL(type.IsNumeric(), true);
  BOOST_CHECK_EQUAL(type.IsPrimitive(), true);
  BOOST_CHECK_EQUAL(type.IsString(), false);
  BOOST_CHECK_EQUAL(type.IsUserType(), false);
  BOOST_CHECK_EQUAL(type.IsEnum(), false);
  BOOST_CHECK_EQUAL(type.IsOpaque(), false);
  BOOST_CHECK_EQUAL(type.IsVLen(), false);
  BOOST_CHECK_EQUAL(type.IsCompound(), false);
  BOOST_CHECK_EQUAL(type.GetPrimitiveName(), "float");
}

// netcdf::type::Int
BOOST_AUTO_TEST_CASE( type_int ) {
  Object object;
  netcdf::type::Int type(object);
  BOOST_CHECK_NE(type.nc_id(), 0);
  BOOST_CHECK_EQUAL(type.id(), netcdf::type::kInt);
  BOOST_CHECK_EQUAL(type.GetSize(), sizeof(int));
  BOOST_CHECK_EQUAL(type.IsFloatingPoint(), false);
  BOOST_CHECK_EQUAL(type.IsIntegral(), true);
  BOOST_CHECK_EQUAL(type.IsNumeric(), true);
  BOOST_CHECK_EQUAL(type.IsPrimitive(), true);
  BOOST_CHECK_EQUAL(type.IsString(), false);
  BOOST_CHECK_EQUAL(type.IsUserType(), false);
  BOOST_CHECK_EQUAL(type.IsEnum(), false);
  BOOST_CHECK_EQUAL(type.IsOpaque(), false);
  BOOST_CHECK_EQUAL(type.IsVLen(), false);
  BOOST_CHECK_EQUAL(type.IsCompound(), false);
  BOOST_CHECK_EQUAL(type.GetPrimitiveName(), "int");
}

// netcdf::type::Int64
BOOST_AUTO_TEST_CASE( type_int64 ) {
  Object object;
  netcdf::type::Int64 type(object);
  BOOST_CHECK_NE(type.nc_id(), 0);
  BOOST_CHECK_EQUAL(type.id(), netcdf::type::kInt64);
  BOOST_CHECK_EQUAL(type.GetSize(), sizeof(long long));
  BOOST_CHECK_EQUAL(type.IsFloatingPoint(), false);
  BOOST_CHECK_EQUAL(type.IsIntegral(), true);
  BOOST_CHECK_EQUAL(type.IsNumeric(), true);
  BOOST_CHECK_EQUAL(type.IsPrimitive(), true);
  BOOST_CHECK_EQUAL(type.IsString(), false);
  BOOST_CHECK_EQUAL(type.IsUserType(), false);
  BOOST_CHECK_EQUAL(type.IsEnum(), false);
  BOOST_CHECK_EQUAL(type.IsOpaque(), false);
  BOOST_CHECK_EQUAL(type.IsVLen(), false);
  BOOST_CHECK_EQUAL(type.IsCompound(), false);
  BOOST_CHECK_EQUAL(type.GetPrimitiveName(), "long long");
}

// netcdf::type::Short
BOOST_AUTO_TEST_CASE( type_short ) {
  Object object;
  netcdf::type::Short type(object);
  BOOST_CHECK_NE(type.nc_id(), 0);
  BOOST_CHECK_EQUAL(type.id(), netcdf::type::kShort);
  BOOST_CHECK_EQUAL(type.GetSize(), sizeof(short));
  BOOST_CHECK_EQUAL(type.IsFloatingPoint(), false);
  BOOST_CHECK_EQUAL(type.IsIntegral(), true);
  BOOST_CHECK_EQUAL(type.IsNumeric(), true);
  BOOST_CHECK_EQUAL(type.IsPrimitive(), true);
  BOOST_CHECK_EQUAL(type.IsString(), false);
  BOOST_CHECK_EQUAL(type.IsUserType(), false);
  BOOST_CHECK_EQUAL(type.IsEnum(), false);
  BOOST_CHECK_EQUAL(type.IsOpaque(), false);
  BOOST_CHECK_EQUAL(type.IsVLen(), false);
  BOOST_CHECK_EQUAL(type.IsCompound(), false);
  BOOST_CHECK_EQUAL(type.GetPrimitiveName(), "short");
}

// netcdf::type::String
BOOST_AUTO_TEST_CASE( type_string ) {
  Object object;
  netcdf::type::String type(object);
  BOOST_CHECK_NE(type.nc_id(), 0);
  BOOST_CHECK_EQUAL(type.id(), netcdf::type::kString);
  BOOST_CHECK_EQUAL(type.GetSize(), 8);
  BOOST_CHECK_EQUAL(type.IsFloatingPoint(), false);
  BOOST_CHECK_EQUAL(type.IsIntegral(), false);
  BOOST_CHECK_EQUAL(type.IsNumeric(), false);
  BOOST_CHECK_EQUAL(type.IsPrimitive(), true);
  BOOST_CHECK_EQUAL(type.IsString(), true);
  BOOST_CHECK_EQUAL(type.IsUserType(), false);
  BOOST_CHECK_EQUAL(type.IsEnum(), false);
  BOOST_CHECK_EQUAL(type.IsOpaque(), false);
  BOOST_CHECK_EQUAL(type.IsVLen(), false);
  BOOST_CHECK_EQUAL(type.IsCompound(), false);
  BOOST_CHECK_EQUAL(type.GetPrimitiveName(), "string");
}

// netcdf::type::UnsignedByte
BOOST_AUTO_TEST_CASE( type_unsigned_byte ) {
  Object object;
  netcdf::type::UnsignedByte type(object);
  BOOST_CHECK_NE(type.nc_id(), 0);
  BOOST_CHECK_EQUAL(type.id(), netcdf::type::kUByte);
  BOOST_CHECK_EQUAL(type.GetSize(), sizeof(unsigned char));
  BOOST_CHECK_EQUAL(type.IsFloatingPoint(), false);
  BOOST_CHECK_EQUAL(type.IsIntegral(), true);
  BOOST_CHECK_EQUAL(type.IsNumeric(), true);
  BOOST_CHECK_EQUAL(type.IsPrimitive(), true);
  BOOST_CHECK_EQUAL(type.IsString(), false);
  BOOST_CHECK_EQUAL(type.IsUserType(), false);
  BOOST_CHECK_EQUAL(type.IsEnum(), false);
  BOOST_CHECK_EQUAL(type.IsOpaque(), false);
  BOOST_CHECK_EQUAL(type.IsVLen(), false);
  BOOST_CHECK_EQUAL(type.IsCompound(), false);
  BOOST_CHECK_EQUAL(type.GetPrimitiveName(), "unsigned byte");
}

// netcdf::type::UnsignedShort
BOOST_AUTO_TEST_CASE( type_unsigned_short ) {
  Object object;
  netcdf::type::UnsignedShort type(object);
  BOOST_CHECK_NE(type.nc_id(), 0);
  BOOST_CHECK_EQUAL(type.id(), netcdf::type::kUShort);
  BOOST_CHECK_EQUAL(type.GetSize(), sizeof(unsigned short));
  BOOST_CHECK_EQUAL(type.IsFloatingPoint(), false);
  BOOST_CHECK_EQUAL(type.IsIntegral(), true);
  BOOST_CHECK_EQUAL(type.IsNumeric(), true);
  BOOST_CHECK_EQUAL(type.IsPrimitive(), true);
  BOOST_CHECK_EQUAL(type.IsString(), false);
  BOOST_CHECK_EQUAL(type.IsUserType(), false);
  BOOST_CHECK_EQUAL(type.IsEnum(), false);
  BOOST_CHECK_EQUAL(type.IsOpaque(), false);
  BOOST_CHECK_EQUAL(type.IsVLen(), false);
  BOOST_CHECK_EQUAL(type.IsCompound(), false);
  BOOST_CHECK_EQUAL(type.GetPrimitiveName(), "unsigned short");
}

// netcdf::type::UnsignedInt
BOOST_AUTO_TEST_CASE( type_unsigned_int ) {
  Object object;
  netcdf::type::UnsignedInt type(object);
  BOOST_CHECK_NE(type.nc_id(), 0);
  BOOST_CHECK_EQUAL(type.id(), netcdf::type::kUInt);
  BOOST_CHECK_EQUAL(type.GetSize(), sizeof(unsigned int));
  BOOST_CHECK_EQUAL(type.IsFloatingPoint(), false);
  BOOST_CHECK_EQUAL(type.IsIntegral(), true);
  BOOST_CHECK_EQUAL(type.IsNumeric(), true);
  BOOST_CHECK_EQUAL(type.IsPrimitive(), true);
  BOOST_CHECK_EQUAL(type.IsString(), false);
  BOOST_CHECK_EQUAL(type.IsUserType(), false);
  BOOST_CHECK_EQUAL(type.IsEnum(), false);
  BOOST_CHECK_EQUAL(type.IsOpaque(), false);
  BOOST_CHECK_EQUAL(type.IsVLen(), false);
  BOOST_CHECK_EQUAL(type.IsCompound(), false);
  BOOST_CHECK_EQUAL(type.GetPrimitiveName(), "unsigned int");
}

// netcdf::type::UnsignedInt64
BOOST_AUTO_TEST_CASE( type_unsigned_int64 ) {
  Object object;
  netcdf::type::UnsignedInt64 type(object);
  BOOST_CHECK_NE(type.nc_id(), 0);
  BOOST_CHECK_EQUAL(type.id(), netcdf::type::kUInt64);
  BOOST_CHECK_EQUAL(type.GetSize(), sizeof(unsigned long long));
  BOOST_CHECK_EQUAL(type.IsFloatingPoint(), false);
  BOOST_CHECK_EQUAL(type.IsIntegral(), true);
  BOOST_CHECK_EQUAL(type.IsNumeric(), true);
  BOOST_CHECK_EQUAL(type.IsPrimitive(), true);
  BOOST_CHECK_EQUAL(type.IsString(), false);
  BOOST_CHECK_EQUAL(type.IsUserType(), false);
  BOOST_CHECK_EQUAL(type.IsEnum(), false);
  BOOST_CHECK_EQUAL(type.IsOpaque(), false);
  BOOST_CHECK_EQUAL(type.IsVLen(), false);
  BOOST_CHECK_EQUAL(type.IsCompound(), false);
  BOOST_CHECK_EQUAL(type.GetPrimitiveName(), "unsigned long long");
}

// netcdf::type::UnsignedEnum
BOOST_AUTO_TEST_CASE( type_enum ) {
  Object object;
  netcdf::type::Enum type(object, "Days", netcdf::type::kShort);
  type.Insert(std::pair<std::string, short>("Monday", 1));
  type.Insert(std::pair<std::string, short>("Tuesday", 2));
  type.Insert(std::pair<std::string, short>("Wednesday", 3));
  type.Insert(std::pair<std::string, short>("Thursday", 4));
  type.Insert(std::pair<std::string, short>("Friday", 5));
  type.Insert(std::pair<std::string, short>("Saturday", 6));
  type.Insert(std::pair<std::string, short>("Sunday", 7));

  BOOST_CHECK_EQUAL(type.GetName(), "Days");
  BOOST_CHECK_EQUAL(type.GetNMembers(), 7);
  BOOST_CHECK_EQUAL(type.GetPrimitive(), netcdf::type::kEnum);
  BOOST_CHECK_EQUAL(type.GetBaseType().GetPrimitive(), netcdf::type::kShort);
  BOOST_CHECK_EQUAL(type.IsFloatingPoint(), false);
  BOOST_CHECK_EQUAL(type.IsIntegral(), false);
  BOOST_CHECK_EQUAL(type.IsNumeric(), false);
  BOOST_CHECK_EQUAL(type.IsPrimitive(), false);
  BOOST_CHECK_EQUAL(type.IsString(), false);
  BOOST_CHECK_EQUAL(type.IsUserType(), true);
  BOOST_CHECK_EQUAL(type.IsEnum(), true);
  BOOST_CHECK_EQUAL(type.IsOpaque(), false);
  BOOST_CHECK_EQUAL(type.IsVLen(), false);
  BOOST_CHECK_EQUAL(type.IsCompound(), false);
  BOOST_CHECK_EQUAL(type.GetPrimitiveName(), "enum");

  for (auto i = 0; i < 7; ++i) {
    std::pair<std::string, short> member = type.Value<short>(i);
    switch (i) {
      case 0:
        BOOST_CHECK_EQUAL(member.first, "Monday");
        BOOST_CHECK_EQUAL(member.second, 1);
        break;
      case 1:
        BOOST_CHECK_EQUAL(member.first, "Tuesday");
        BOOST_CHECK_EQUAL(member.second, 2);
        break;
      case 2:
        BOOST_CHECK_EQUAL(member.first, "Wednesday");
        BOOST_CHECK_EQUAL(member.second, 3);
        break;
      case 3:
        BOOST_CHECK_EQUAL(member.first, "Thursday");
        BOOST_CHECK_EQUAL(member.second, 4);
        break;
      case 4:
        BOOST_CHECK_EQUAL(member.first, "Friday");
        BOOST_CHECK_EQUAL(member.second, 5);
        break;
      case 5:
        BOOST_CHECK_EQUAL(member.first, "Saturday");
        BOOST_CHECK_EQUAL(member.second, 6);
        break;
      case 6:
        BOOST_CHECK_EQUAL(member.first, "Sunday");
        BOOST_CHECK_EQUAL(member.second, 7);
        break;
    }
  }

  for (auto& item : type.Values<short>()) {
    switch (item.second) {
      case 1:
        BOOST_CHECK_EQUAL(item.first, "Monday");
        break;
      case 2:
        BOOST_CHECK_EQUAL(item.first, "Tuesday");
        break;
      case 3:
        BOOST_CHECK_EQUAL(item.first, "Wednesday");
        break;
      case 4:
        BOOST_CHECK_EQUAL(item.first, "Thursday");
        break;
      case 5:
        BOOST_CHECK_EQUAL(item.first, "Friday");
        break;
      case 6:
        BOOST_CHECK_EQUAL(item.first, "Saturday");
        break;
      case 7:
        BOOST_CHECK_EQUAL(item.first, "Sunday");
        break;
    }
  }
}

// netcdf::type::VLen
BOOST_AUTO_TEST_CASE( type_vlen ) {
  Object object;
  netcdf::type::VLen type(object, "MyVarLen", netcdf::type::Int(object));

  BOOST_CHECK_EQUAL(type.GetPrimitive(), netcdf::type::kVLen);
  BOOST_CHECK_EQUAL(type.GetBaseType().GetPrimitive(), netcdf::type::kInt);
  BOOST_CHECK_EQUAL(type.GetName(), "MyVarLen");
  BOOST_CHECK_EQUAL(type.GetSize(), 16);
  BOOST_CHECK_EQUAL(type.IsFloatingPoint(), false);
  BOOST_CHECK_EQUAL(type.IsIntegral(), false);
  BOOST_CHECK_EQUAL(type.IsNumeric(), false);
  BOOST_CHECK_EQUAL(type.IsPrimitive(), false);
  BOOST_CHECK_EQUAL(type.IsString(), false);
  BOOST_CHECK_EQUAL(type.IsUserType(), true);
  BOOST_CHECK_EQUAL(type.IsEnum(), false);
  BOOST_CHECK_EQUAL(type.IsOpaque(), false);
  BOOST_CHECK_EQUAL(type.IsVLen(), true);
  BOOST_CHECK_EQUAL(type.IsCompound(), false);
  BOOST_CHECK_EQUAL(type.GetPrimitiveName(), "vlen");
}

// netcdf::type::Opaque
BOOST_AUTO_TEST_CASE( type_opaque ) {
  Object object;
  netcdf::type::Opaque type(object, "MyOpaque", 64);

  BOOST_CHECK_EQUAL(type.GetPrimitive(), netcdf::type::kOpaque);
  BOOST_CHECK_EQUAL(type.GetName(), "MyOpaque");
  BOOST_CHECK_EQUAL(type.GetSize(), 64);
  BOOST_CHECK_EQUAL(type.IsFloatingPoint(), false);
  BOOST_CHECK_EQUAL(type.IsIntegral(), false);
  BOOST_CHECK_EQUAL(type.IsNumeric(), false);
  BOOST_CHECK_EQUAL(type.IsPrimitive(), false);
  BOOST_CHECK_EQUAL(type.IsString(), false);
  BOOST_CHECK_EQUAL(type.IsUserType(), true);
  BOOST_CHECK_EQUAL(type.GetPrimitiveName(), "opaque");
  BOOST_CHECK_EQUAL(type.IsEnum(), false);
  BOOST_CHECK_EQUAL(type.IsOpaque(), true);
  BOOST_CHECK_EQUAL(type.IsVLen(), false);
  BOOST_CHECK_EQUAL(type.IsCompound(), false);
}

// netcdf::type::Compound
BOOST_AUTO_TEST_CASE( type_compound ) {
  typedef struct Compound_ {
    char a;
    short b;
    int c;
    float d;
    double e[8];
  } Compound;

  Object object;
  netcdf::type::Compound type(object, "MyCompound", sizeof(Compound));
  type.InsertMember("a", NC_COMPOUND_OFFSET(Compound, a),
                    netcdf::type::Char(object));
  type.InsertMember("b", NC_COMPOUND_OFFSET(Compound, b),
                    netcdf::type::Short(object));
  type.InsertMember("c", NC_COMPOUND_OFFSET(Compound, c),
                    netcdf::type::Int(object));
  type.InsertMember("d", NC_COMPOUND_OFFSET(Compound, d),
                    netcdf::type::Float(object));
  type.InsertMember("e", NC_COMPOUND_OFFSET(Compound, e),
                    netcdf::type::Double(object), std::vector<int>(1, 8));

  BOOST_CHECK_EQUAL(type.GetNMembers(), 5);
  BOOST_CHECK_EQUAL(type.GetDims(0).size(), 0);
  BOOST_CHECK_EQUAL(type.GetDims(1).size(), 0);
  BOOST_CHECK_EQUAL(type.GetDims(2).size(), 0);
  BOOST_CHECK_EQUAL(type.GetDims(3).size(), 0);
  BOOST_CHECK_EQUAL(type.GetDims(4).size(), 1);
  BOOST_CHECK_EQUAL(type.GetDims(4).at(0), 8);
  BOOST_CHECK_EQUAL(type.GetMemberName(0), "a");
  BOOST_CHECK_EQUAL(type.GetMemberName(1), "b");
  BOOST_CHECK_EQUAL(type.GetMemberName(2), "c");
  BOOST_CHECK_EQUAL(type.GetMemberName(3), "d");
  BOOST_CHECK_EQUAL(type.GetMemberName(4), "e");
  BOOST_CHECK_EQUAL(type.GetMemberIndex("a"), 0);
  BOOST_CHECK_EQUAL(type.GetMemberIndex("b"), 1);
  BOOST_CHECK_EQUAL(type.GetMemberIndex("c"), 2);
  BOOST_CHECK_EQUAL(type.GetMemberIndex("d"), 3);
  BOOST_CHECK_EQUAL(type.GetMemberIndex("e"), 4);
  BOOST_CHECK_EQUAL(type.GetMemberOffset(0), NC_COMPOUND_OFFSET(Compound, a));
  BOOST_CHECK_EQUAL(type.GetMemberOffset(1), NC_COMPOUND_OFFSET(Compound, b));
  BOOST_CHECK_EQUAL(type.GetMemberOffset(2), NC_COMPOUND_OFFSET(Compound, c));
  BOOST_CHECK_EQUAL(type.GetMemberOffset(3), NC_COMPOUND_OFFSET(Compound, d));
  BOOST_CHECK_EQUAL(type.GetMemberOffset(4), NC_COMPOUND_OFFSET(Compound, e));
  BOOST_CHECK_EQUAL(type.GetMemberClass(0).GetPrimitive(), netcdf::type::kChar);
  BOOST_CHECK_EQUAL(type.GetMemberClass(1).GetPrimitive(),
                    netcdf::type::kShort);
  BOOST_CHECK_EQUAL(type.GetMemberClass(2).GetPrimitive(), netcdf::type::kInt);
  BOOST_CHECK_EQUAL(type.GetMemberClass(3).GetPrimitive(),
                    netcdf::type::kFloat);
  BOOST_CHECK_EQUAL(type.GetMemberClass(4).GetPrimitive(),
                    netcdf::type::kDouble);
  BOOST_CHECK_EQUAL(type.GetPrimitive(), netcdf::type::kCompound);
  BOOST_CHECK_EQUAL(type.GetName(), "MyCompound");
  BOOST_CHECK_EQUAL(type.GetSize(), sizeof(Compound));
  BOOST_CHECK_EQUAL(type.IsFloatingPoint(), false);
  BOOST_CHECK_EQUAL(type.IsIntegral(), false);
  BOOST_CHECK_EQUAL(type.IsNumeric(), false);
  BOOST_CHECK_EQUAL(type.IsPrimitive(), false);
  BOOST_CHECK_EQUAL(type.IsString(), false);
  BOOST_CHECK_EQUAL(type.IsUserType(), true);
  BOOST_CHECK_EQUAL(type.IsEnum(), false);
  BOOST_CHECK_EQUAL(type.IsOpaque(), false);
  BOOST_CHECK_EQUAL(type.IsVLen(), false);
  BOOST_CHECK_EQUAL(type.IsCompound(), true);
  BOOST_CHECK_EQUAL(type.GetPrimitiveName(), "compound");
}

BOOST_AUTO_TEST_CASE( test_comparison ) {
  Object object;
  netcdf::type::Byte a(object);
  netcdf::type::Int b(object);

  BOOST_CHECK(a == a);
  BOOST_CHECK(not (a != a));
  BOOST_CHECK(not (a != a));
  BOOST_CHECK((a != b));
}

BOOST_AUTO_TEST_SUITE_END( )
