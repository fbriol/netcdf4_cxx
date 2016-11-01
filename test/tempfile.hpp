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

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/system/error_code.hpp>
#include <netcdf/netcdf.hpp>
#include <netcdf/object.hpp>
#include <netcdf.h>
#include <iostream>
#include <string>

class TempFile {
 private:
  boost::filesystem::path temp_;
 public:
  TempFile()
      : temp_(boost::filesystem::unique_path()) {
  }

  explicit TempFile(const std::string& path)
      : temp_(path) {
  }

  std::string Path() const {
    return temp_.native();
  }

  void Unlink() {
    boost::system::error_code ec;
    boost::filesystem::remove(temp_, ec);
  }

  void New() {
    Unlink();
    temp_ = boost::filesystem::unique_path();
  }

  ~TempFile() {
    Unlink();
  }
};

class Object : public netcdf::Object {
 private:
  TempFile temp_file_;

 public:
  Object()
      : netcdf::Object(),
        temp_file_() {
    int flag = NC_CLOBBER;
    netcdf::Check(nc_set_default_format(NC_FORMAT_NETCDF4, NULL));
    netcdf::Check(nc_create(temp_file_.Path().c_str(), flag, &nc_id_));
  }

  Object(const std::string& path)
      : netcdf::Object(),
        temp_file_(path) {
    int flag = NC_CLOBBER;
    netcdf::Check(nc_set_default_format(NC_FORMAT_NETCDF4, NULL));
    netcdf::Check(nc_create(temp_file_.Path().c_str(), flag, &nc_id_));
  }

  ~Object() {
    nc_close(nc_id_);
  }
};
