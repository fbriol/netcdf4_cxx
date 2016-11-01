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
#include <netcdf4_cxx/dataset.hpp>
#include <netcdf4_cxx/object.hpp>

#include "tempfile.hpp"

BOOST_AUTO_TEST_SUITE (test_dataset)

BOOST_AUTO_TEST_CASE( test_container ) {
  Object object;
  std::vector<std::string> strings;
  for (char i = 'a'; i <= 'x'; ++i) {
    strings.push_back(std::string("att_") + i);
  }
  for (short i = 0; i < static_cast<short>(strings.size()); ++i) {
    netcdf::Attribute(object, strings[i]).Write(netcdf::type::Short(object),
                                                std::vector<short>( { i }));
  }
  netcdf::DataSet ds(object);
  std::list<netcdf::Attribute> atts = ds.GetAttributes();
  BOOST_CHECK_EQUAL(atts.size(), strings.size());
  BOOST_CHECK_EQUAL(atts.front().name(), "att_a");
  BOOST_CHECK_EQUAL(atts.back().name(), "att_x");
}


BOOST_AUTO_TEST_CASE( test_case ) {
  Object object;
  netcdf::Attribute(object, "Attribute").Write(netcdf::type::Short(object),
                                               std::vector<short>( { 1 }));
  netcdf::DataSet ds(object);
  BOOST_CHECK_NO_THROW(ds.FindAttribute("attribute"));
  BOOST_CHECK_EQUAL(ds.AttributeExists("Attribute"), true);
  BOOST_CHECK_EQUAL(ds.AttributeExists("attribute"), false);
  BOOST_CHECK_EQUAL(ds.AttributeExists("attribute", true), true);
  BOOST_CHECK(ds.FindAttribute("attribute").get() == nullptr);
}

BOOST_AUTO_TEST_CASE( test_add_get ) {
  Object object;
  netcdf::DataSet ds(object);
  ds.AddAttribute("Attribute").WriteText("Lorem ipsum dolor sit amet");
  auto att = ds.FindAttribute("Attribute");
  BOOST_REQUIRE(att.get() != nullptr);
  BOOST_CHECK_EQUAL(att->name(), "Attribute");
  BOOST_CHECK_EQUAL(att->ReadText(), "Lorem ipsum dolor sit amet");
}

BOOST_AUTO_TEST_CASE( test_delete ) {
  Object object;
  netcdf::DataSet ds(object);
  ds.AddAttribute("Attribute").WriteText("Lorem ipsum dolor sit amet");
  BOOST_CHECK_EQUAL(ds.AttributeExists("Attribute"), true);
  ds.RemoveAttribute("Attribute");
  BOOST_CHECK_EQUAL(ds.AttributeExists("Attribute"), false);
}


BOOST_AUTO_TEST_SUITE_END( )
