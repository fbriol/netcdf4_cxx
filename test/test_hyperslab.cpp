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
#include <netcdf4_cxx/hyperslab.hpp>

BOOST_AUTO_TEST_SUITE (test_range)

BOOST_AUTO_TEST_CASE( test_default_constructor ) {
  netcdf::Range object;
  BOOST_CHECK_EQUAL(object.GetSize(), 0);
  BOOST_CHECK_THROW(object.Item(5), std::out_of_range);
  BOOST_CHECK_THROW(object.Index(5), std::out_of_range);
  BOOST_CHECK_EQUAL(object.Contains(5), false);
  BOOST_CHECK_EQUAL(object.First(), 0);
  BOOST_CHECK_EQUAL(object.Last(), 0);
  BOOST_CHECK_EQUAL(object.IsEmpty(), true);
}

BOOST_AUTO_TEST_CASE( test_built_from_shape ) {
  netcdf::Range object(10);
  BOOST_CHECK_EQUAL(object.GetSize(), 10);
  BOOST_CHECK_EQUAL(object.Index(5), 5);
  BOOST_CHECK_EQUAL(object.Item(5), 5);
  BOOST_CHECK_EQUAL(object.Contains(5), true);
  BOOST_CHECK_EQUAL(object.Contains(10), false);
  BOOST_CHECK_EQUAL(object.First(), 0);
  BOOST_CHECK_EQUAL(object.Last(), 9);
  BOOST_CHECK_EQUAL(object.IsEmpty(), false);

  BOOST_REQUIRE(object.GetSize() == 10);
  std::vector<size_t> ref( { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }), res;
  res = static_cast<std::vector<size_t> >(object);
  BOOST_CHECK_EQUAL_COLLECTIONS(ref.begin(), ref.end(), res.begin(), res.end());
}

BOOST_AUTO_TEST_CASE( test_built_from_range ) {
  netcdf::Range object(2, 10);
  BOOST_CHECK_EQUAL(object.GetSize(), 8);
  BOOST_CHECK_EQUAL(object.Index(5), 3);
  BOOST_CHECK_EQUAL(object.Item(5), 7);
  BOOST_CHECK_EQUAL(object.Contains(5), true);
  BOOST_CHECK_EQUAL(object.Contains(10), false);
  BOOST_CHECK_EQUAL(object.First(), 2);
  BOOST_CHECK_EQUAL(object.Last(), 9);
  BOOST_CHECK_EQUAL(object.IsEmpty(), false);

  BOOST_REQUIRE(object.GetSize() == 8);
  std::vector<size_t> ref( { 2, 3, 4, 5, 6, 7, 8, 9 });
  std::vector<size_t> res = static_cast<std::vector<size_t> >(object);
  BOOST_CHECK_EQUAL_COLLECTIONS(ref.begin(), ref.end(), res.begin(), res.end());
}

BOOST_AUTO_TEST_CASE( test_built_from_complete_definition ) {
  netcdf::Range object(12, 53, 7);
  BOOST_CHECK_EQUAL(object.GetSize(), 6);
  BOOST_CHECK_EQUAL(object.Index(26), 2);
  BOOST_CHECK_EQUAL(object.Item(4), 40);
  BOOST_CHECK_EQUAL(object.Contains(5), false);
  BOOST_CHECK_EQUAL(object.Contains(18), false);
  BOOST_CHECK_EQUAL(object.Contains(19), true);
  BOOST_CHECK_EQUAL(object.First(), 12);
  BOOST_CHECK_EQUAL(object.Last(), 52);
  BOOST_CHECK_EQUAL(object.IsEmpty(), false);

  BOOST_CHECK_THROW(object.Item(6), std::out_of_range);
  BOOST_CHECK_THROW(object.Item(7), std::out_of_range);
  BOOST_CHECK_THROW(object.Index(10), std::out_of_range);
  BOOST_CHECK_THROW(object.Index(48), std::out_of_range);

  BOOST_REQUIRE(object.GetSize() == 6);
  std::vector<size_t> ref( { 12, 19, 26, 33, 40, 47 });
  std::vector<size_t> res = static_cast<std::vector<size_t> >(object);
  BOOST_CHECK_EQUAL_COLLECTIONS(ref.begin(), ref.end(), res.begin(), res.end());
}

BOOST_AUTO_TEST_CASE( test_built_from_invalid_definition ) {
  netcdf::Range object(12, 1, 1);
  BOOST_CHECK_EQUAL(object.GetSize(), 0);
  BOOST_CHECK_EQUAL(object.Contains(5), false);
  BOOST_CHECK_EQUAL(object.Contains(18), false);
  BOOST_CHECK_EQUAL(object.Contains(19), false);
  BOOST_CHECK_THROW(object.Item(6), std::out_of_range);
  BOOST_CHECK_THROW(object.Index(48), std::out_of_range);
  BOOST_CHECK_EQUAL(object.First(), 0);
  BOOST_CHECK_EQUAL(object.Last(), 0);
  BOOST_CHECK_EQUAL(object.IsEmpty(), true);
  std::vector<size_t> ref;
  std::vector<size_t> res = static_cast<std::vector<size_t> >(object);
  BOOST_CHECK_EQUAL_COLLECTIONS(ref.begin(), ref.end(), res.begin(), res.end());
}

BOOST_AUTO_TEST_CASE( test_built_from_reverse_range ) {
  netcdf::Range object(12, 1, -1);
  BOOST_CHECK_EQUAL(object.GetSize(), 11);
  BOOST_CHECK_EQUAL(object.Contains(5), true);
  BOOST_CHECK_EQUAL(object.Contains(1), false);
  BOOST_CHECK_EQUAL(object.Index(7), 5);
  BOOST_CHECK_EQUAL(object.Item(7), 5);
  BOOST_CHECK_EQUAL(object.First(), 12);
  BOOST_CHECK_EQUAL(object.Last(), 2);
  BOOST_CHECK_EQUAL(object.IsEmpty(), false);
  std::vector<size_t> ref( { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 });
  std::vector<size_t> res = static_cast<std::vector<size_t> >(object);
  BOOST_CHECK_EQUAL_COLLECTIONS(ref.begin(), ref.end(), res.begin(), res.end());
}

BOOST_AUTO_TEST_SUITE_END( )

BOOST_AUTO_TEST_SUITE (test_hyperslab)

BOOST_AUTO_TEST_CASE( default_constructor ) {
  netcdf::Hyperslab object;
  BOOST_CHECK_EQUAL(object.IsEmpty(), true);
  BOOST_CHECK_EQUAL(object.OnlyAdjacent(), true);
  BOOST_CHECK_EQUAL(object.GetSize(), 0);
  BOOST_CHECK_EQUAL(object.GetRank(), 0);
  BOOST_CHECK_EQUAL(object.GetSizeList().size(), 0);

  std::vector<size_t> shape( { 2, 2 });

  BOOST_CHECK(!(object <= shape));
  BOOST_CHECK(object > shape);
}

BOOST_AUTO_TEST_CASE( constructor_with_shape ) {
  std::vector<size_t> shape( { 2, 2 });
  netcdf::Hyperslab object(shape);
  BOOST_CHECK_EQUAL(object.IsEmpty(), false);
  BOOST_CHECK_EQUAL(object.OnlyAdjacent(), true);
  BOOST_CHECK_EQUAL(object.GetSize(), 4);
  BOOST_CHECK_EQUAL(object.GetRank(), 2);
  for (auto& item: object.GetSizeList()) {
    BOOST_CHECK_EQUAL(item, 2);
  }
  BOOST_CHECK((object <= shape));
  BOOST_CHECK(!(object > shape));
  shape = std::vector<size_t> ( { 4, 4 });
  BOOST_CHECK((object <= shape));
  BOOST_CHECK(!(object > shape));
  shape = std::vector<size_t> ( { 1, 1 });
  BOOST_CHECK(!(object <= shape));
  BOOST_CHECK(object > shape);
}

BOOST_AUTO_TEST_CASE( constructor_with_range ) {
  std::vector<size_t> start( { 1, 3 });
  std::vector<size_t> shape( { 5, 5 });
  netcdf::Hyperslab object(start, shape);
  BOOST_CHECK_EQUAL(object.IsEmpty(), false);
  BOOST_CHECK_EQUAL(object.OnlyAdjacent(), true);
  BOOST_CHECK_EQUAL(object.GetSize(), 8);
  BOOST_CHECK_EQUAL(object.GetRank(), 2);
  std::vector<size_t> size = object.GetSizeList();
  BOOST_CHECK_EQUAL(size[0], 4);
  BOOST_CHECK_EQUAL(size[1], 2);
  BOOST_CHECK((object <= shape));
  BOOST_CHECK(!(object > shape));
  shape = std::vector<size_t> ( { 4, 4 });
  BOOST_CHECK(object <= shape);
  BOOST_CHECK(!(object > shape));
  shape = std::vector<size_t> ( { 1, 1 });
  BOOST_CHECK(!(object <= shape));
  BOOST_CHECK(object > shape);
}

BOOST_AUTO_TEST_CASE( constructor_with_strides ) {
  std::vector<size_t> start( { 7, 13, 15 });
  std::vector<size_t> shape( { 15, 30, 60 });
  std::vector<ptrdiff_t> step( { 3, 7, 11 });
  netcdf::Hyperslab object(start, shape, step);
  BOOST_CHECK_EQUAL(object.IsEmpty(), false);
  BOOST_CHECK_EQUAL(object.OnlyAdjacent(), false);
  BOOST_CHECK_EQUAL(object.GetSize(), 45);
  BOOST_CHECK_EQUAL(object.GetRank(), 3);
  std::vector<size_t> size = object.GetSizeList();
  BOOST_CHECK_EQUAL(size[0], 3);
  BOOST_CHECK_EQUAL(size[1], 3);
  BOOST_CHECK_EQUAL(size[2], 5);
  BOOST_CHECK((object <= shape));
  BOOST_CHECK(!(object > shape));
  shape = std::vector<size_t> ( { 4, 4, 5 });
  BOOST_CHECK(object <= shape);
  BOOST_CHECK(!(object > shape));
  shape = std::vector<size_t> ( { 4, 4, 4 });
  BOOST_CHECK(!(object <= shape));
  BOOST_CHECK(object > shape);
}

BOOST_AUTO_TEST_SUITE_END( )
