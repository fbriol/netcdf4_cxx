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
#include <netcdf4_cxx/dimension.hpp>
#include <netcdf4_cxx/object.hpp>
#include <netcdf.h>

#include "tempfile.hpp"

BOOST_AUTO_TEST_SUITE (test_dimension)

BOOST_AUTO_TEST_CASE( test_accessor ) {
  Object object;
  int dimid;
  nc_def_dim(object.nc_id(), "__x__", 100, &dimid);
  netcdf::Dimension x(object, dimid);

  nc_def_dim(object.nc_id(), "__y__", NC_UNLIMITED, &dimid);
  netcdf::Dimension y(object, dimid);

  BOOST_CHECK_EQUAL(x.IsUnlimited(), false);
  BOOST_CHECK_EQUAL(y.IsUnlimited(), true);

  BOOST_CHECK_EQUAL(x.GetLength(), 100);
  BOOST_CHECK_EQUAL(y.GetLength(), 0);

  BOOST_CHECK_EQUAL(x.GetShortName(), "__x__");
  BOOST_CHECK_EQUAL(y.GetShortName(), "__y__");

  x.Rename("x");
  BOOST_CHECK_EQUAL(x.GetShortName(), "x");
}


BOOST_AUTO_TEST_SUITE_END( )
