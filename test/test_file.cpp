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
#include <netcdf4_cxx/abstract_dataset.hpp>
#include <netcdf4_cxx/file.hpp>

#include "tempfile.hpp"

BOOST_AUTO_TEST_SUITE(test_file)

BOOST_AUTO_TEST_CASE(type_constructors) {
  netcdf::File file;
  BOOST_CHECK_THROW(file.AddDimension("a", 10), netcdf::Error);

  BOOST_CHECK_NO_THROW(file.Open("test", "w", false, true));
  file.Close();

  TempFile temp;
  BOOST_CHECK_NO_THROW(file.Open(temp.Path(), "w", false, false, false,
                                 netcdf::Format::kNetCdf4));
  BOOST_CHECK_EQUAL(file.GetFilePath(), temp.Path());
  BOOST_CHECK(file.GetFormat() == netcdf::Format::kNetCdf4);
  file.Close();

  BOOST_CHECK_THROW(file.Open(temp.Path(), "w", false, false, false,
                              netcdf::Format::kNetCdf4),
                    netcdf::Error);
  BOOST_CHECK_NO_THROW(file.Open(temp.Path(), "w", true, false, false,
                                 netcdf::Format::kNetCdf4));
  file.Close();

  BOOST_CHECK_NO_THROW(file.Open(temp.Path(), "w", true, false, false,
                                 netcdf::Format::kClassicNetCdf3));
  netcdf::Dimension dim = file.AddDimension("x", 10);
  std::vector<netcdf::Dimension> dims({dim});
  netcdf::Variable var = file.AddVariable("var", netcdf::type::Int(file), dims);
  std::valarray<int> values({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  BOOST_CHECK_THROW(var.Write(values), netcdf::Error);
  file.LeaveDefineMode();
  BOOST_CHECK_NO_THROW(var.Write(values));
  TempFile file_copy;
  netcdf::File copy;
  copy.Open(file_copy.Path(), "w", false, false, false);
  file.Copy(copy);
  auto src_dims = file.GetDimensions();
  auto dst_dims = copy.GetDimensions();
  BOOST_CHECK_EQUAL(src_dims.size(), dst_dims.size());
  for (size_t ix = 0; ix < src_dims.size(); ++ix) {
    BOOST_CHECK_EQUAL(src_dims[ix].GetLength(), dst_dims[ix].GetLength());
  }

  auto src_vars = file.GetVariables();
  auto dst_vars = copy.GetVariables();
  BOOST_CHECK_EQUAL(src_vars.size(), dst_vars.size());
  var = dst_vars.front();
  std::valarray<int> dst_var = var.Read<int>();
  BOOST_CHECK_EQUAL(dst_var.size(), values.size());
  for (size_t ix = 0; ix < dst_var.size(); ++ix) {
    BOOST_CHECK_EQUAL(dst_var[ix], values[ix]);
  }
}

// BOOST_AUTO_TEST_CASE( test_copy ) {
//   netcdf::File src, tgt;
//   src.Open("/Users/fbriol/Documents/workspace/netcdfcpp/test/test.nc");
//   tgt.Open("/tmp/test.nc", "w", true);
//   src.Copy(tgt);
// }

BOOST_AUTO_TEST_SUITE_END()
