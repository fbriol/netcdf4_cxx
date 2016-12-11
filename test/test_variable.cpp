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
#include <netcdf4_cxx/object.hpp>
#include <netcdf4_cxx/variable.hpp>

#include "tempfile.hpp"

BOOST_AUTO_TEST_SUITE(test_variable)

BOOST_AUTO_TEST_CASE(test_accessor) {
  Object object;
  std::vector<size_t> shape({4, 16, 256});
  std::vector<int> dimid(shape.size());

  nc_def_dim(object.nc_id(), "x", shape[0], &dimid[0]);
  nc_def_dim(object.nc_id(), "y", shape[1], &dimid[1]);
  nc_def_dim(object.nc_id(), "z", shape[2], &dimid[2]);

  int varid;
  nc_def_var(object.nc_id(), "my_variable", NC_SHORT, shape.size(), &dimid[0],
             &varid);

  netcdf::Variable var(object, varid);
  BOOST_CHECK(var.GetDataType().GetPrimitive() ==
              netcdf::type::Primitive::kShort);
  BOOST_CHECK_EQUAL(var.GetRank(), shape.size());

  for (auto& item : var.GetDimensions()) {
    if (item.GetShortName() == "x")
      BOOST_CHECK_EQUAL(item.GetLength(), shape[0]);
    else if (item.GetShortName() == "y")
      BOOST_CHECK_EQUAL(item.GetLength(), shape[1]);
    else if (item.GetShortName() == "z")
      BOOST_CHECK_EQUAL(item.GetLength(), shape[2]);
    else
      BOOST_FAIL("unexpected dimension:" + item.GetShortName());
  }
  auto res = var.GetShape();
  BOOST_CHECK_EQUAL_COLLECTIONS(shape.begin(), shape.end(), res.begin(),
                                res.end());
  BOOST_CHECK_EQUAL(var.GetSize(), shape[0] * shape[1] * shape[2]);
  BOOST_CHECK_EQUAL(var.GetShortName(), "my_variable");
  BOOST_CHECK_EQUAL(var.IsCoordinate(), false);
  BOOST_CHECK_EQUAL(var.IsUnlimited(), false);

  var.Rename("MyVariable");
  BOOST_CHECK_EQUAL(var.GetShortName(), "MyVariable");
}

BOOST_AUTO_TEST_CASE(test_coordinate) {
  Object object;
  std::vector<size_t> length({4, 16, 256});
  std::vector<int> dimid(3);
  std::vector<int> varid(2);

  nc_def_dim(object.nc_id(), "x", length[0], &dimid[0]);
  nc_def_dim(object.nc_id(), "y", length[1], &dimid[1]);
  nc_def_dim(object.nc_id(), "z", length[2], &dimid[2]);

  nc_def_var(object.nc_id(), "x", NC_SHORT, 1, &dimid[0], &varid[0]);
  nc_def_var(object.nc_id(), "y", NC_CHAR, 2, &dimid[1], &varid[1]);

  netcdf::Variable x(object, varid[0]);
  netcdf::Variable y(object, varid[1]);
  BOOST_CHECK_EQUAL(x.IsCoordinate(), true);
  BOOST_CHECK_EQUAL(y.IsCoordinate(), true);
}

BOOST_AUTO_TEST_CASE(test_unlimited) {
  Object object;
  std::vector<int> dimid(2);
  std::vector<int> varid(2);

  nc_def_dim(object.nc_id(), "x", 64, &dimid[0]);
  nc_def_dim(object.nc_id(), "y", NC_UNLIMITED, &dimid[1]);

  nc_def_var(object.nc_id(), "x", NC_SHORT, 1, &dimid[0], &varid[0]);
  nc_def_var(object.nc_id(), "y", NC_SHORT, 1, &dimid[1], &varid[1]);

  netcdf::Variable x(object, varid[0]);
  netcdf::Variable y(object, varid[1]);
  BOOST_CHECK_EQUAL(x.IsUnlimited(), false);
  BOOST_CHECK_EQUAL(y.IsUnlimited(), true);
}

#define TEST_RW(NAME, TYPE, NCTYPE)                                           \
  BOOST_AUTO_TEST_CASE(NAME) {                                                \
    Object object;                                                            \
    std::vector<size_t> shape({64, 128});                                     \
    std::vector<int> dimid(2);                                                \
    int varid;                                                                \
                                                                              \
    nc_def_dim(object.nc_id(), "x", shape[0], &dimid[0]);                     \
    nc_def_dim(object.nc_id(), "y", NC_UNLIMITED, &dimid[1]);                 \
    nc_def_var(object.nc_id(), "m", NCTYPE, shape.size(), &dimid[0], &varid); \
                                                                              \
    netcdf::Variable netcdf_var(object, varid);                               \
    std::valarray<TYPE> cpp_var_ref(64 * 128);                                \
    std::valarray<TYPE> cpp_var_res;                                          \
                                                                              \
    for (size_t i = 0; i < cpp_var_res.size(); ++i) {                         \
      cpp_var_ref[i] = static_cast<TYPE>(i);                                  \
    }                                                                         \
                                                                              \
    netcdf_var.Write(netcdf::Hyperslab(shape), cpp_var_ref);                  \
    cpp_var_res = netcdf_var.Read<TYPE>();                                    \
                                                                              \
    BOOST_REQUIRE(cpp_var_ref.size() == cpp_var_res.size());                  \
    for (size_t i = 0; i < cpp_var_ref.size(); ++i) {                         \
      BOOST_CHECK_EQUAL(cpp_var_ref[i], cpp_var_res[i]);                      \
    }                                                                         \
  }

TEST_RW(test_byte, signed char, NC_BYTE)
TEST_RW(test_ubyte, unsigned char, NC_UBYTE)
TEST_RW(test_short, short, NC_SHORT)
TEST_RW(test_ushort, unsigned short, NC_USHORT)
TEST_RW(test_int, int, NC_INT)
TEST_RW(test_uint, unsigned int, NC_UINT)
TEST_RW(test_int64, long long, NC_INT64)
TEST_RW(test_uint64, unsigned long long, NC_UINT64)
TEST_RW(test_float, float, NC_FLOAT)
TEST_RW(test_double, double, NC_DOUBLE)

BOOST_AUTO_TEST_CASE(test_compound) {
  Object object;
  std::vector<size_t> shape({64, 128});
  std::vector<int> dimid(2);
  int varid;

  // dimensions
  nc_def_dim(object.nc_id(), "x", shape[0], &dimid[0]);
  nc_def_dim(object.nc_id(), "y", NC_UNLIMITED, &dimid[1]);

  // Compound type
  struct Coordinate {
    size_t i;
    size_t j;
  };
  netcdf::type::Compound type(object, "Coordinate", sizeof(Coordinate));
  type.InsertMember("i", NC_COMPOUND_OFFSET(Coordinate, i),
                    netcdf::type::UnsignedInt64(object));
  type.InsertMember("j", NC_COMPOUND_OFFSET(Coordinate, j),
                    netcdf::type::UnsignedInt64(object));

  // Variable
  nc_def_var(object.nc_id(), "m", type.id(), shape.size(), &dimid[0], &varid);

  netcdf::Variable netcdf_var(object, varid);
  std::valarray<Coordinate> cpp_var_ref(64 * 128);
  std::valarray<Coordinate> cpp_var_res;

  // Fill the variable
  for (size_t i = 0; i < shape[0]; ++i) {
    for (size_t j = 0; j < shape[1]; ++j) {
      cpp_var_ref[j + i * shape[1]] = Coordinate({i, j});
    }
  }

  // Write/read all the data
  netcdf_var.Write(netcdf::Hyperslab(shape), cpp_var_ref);
  cpp_var_res = netcdf_var.Read<Coordinate>();

  BOOST_REQUIRE(cpp_var_ref.size() == cpp_var_res.size());
  for (size_t i = 0; i < shape[0]; ++i) {
    for (size_t j = 0; j < shape[1]; ++j) {
      const Coordinate& ref = cpp_var_ref[j + i * shape[1]];
      const Coordinate& res = cpp_var_res[j + i * shape[1]];
      BOOST_CHECK_EQUAL(ref.j, res.j);
      BOOST_CHECK_EQUAL(ref.i, res.i);
    }
  }

  // Read a region
  netcdf::Hyperslab select(std::vector<size_t>({12, 31}),
                           std::vector<size_t>({48, 112}),
                           std::vector<ptrdiff_t>({4, 7}));
  cpp_var_res = netcdf_var.Read<Coordinate>(select);

  BOOST_REQUIRE(cpp_var_res.size() == select.GetSize());
  netcdf::Range rx = select.GetRange(0);
  netcdf::Range ry = select.GetRange(1);

  for (size_t i = 0; i < select.GetSize(0); ++i) {
    for (size_t j = 0; j < select.GetSize(1); ++j) {
      const Coordinate& res = cpp_var_res[j + i * select.GetSize(1)];
      BOOST_CHECK_EQUAL(res.i, rx.Item(i));
      BOOST_CHECK_EQUAL(res.j, ry.Item(j));
    }
  }
}

BOOST_AUTO_TEST_SUITE_END()
