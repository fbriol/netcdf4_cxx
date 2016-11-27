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


#include <netcdf4_cxx/file.hpp>

namespace netcdf {

void File::Open(const std::string& filename, const std::string& mode,
                bool clobber, const bool diskless, const bool persist,
                const Format format) {
  int flags, ident;

  nc_close(nc_id_);

  if (mode == "w" || mode == "ws") {
    Check(nc_set_default_format(static_cast<int>(format), NULL));

    flags = clobber ? NC_CLOBBER : NC_NOCLOBBER;

    if (mode[mode.size() - 1] == 's')
      flags |= NC_SHARE;

    if (diskless) {
      flags |= NC_DISKLESS;

      if (persist)
        flags |= NC_WRITE;
    }
  } else if (mode == "r") {
    flags = NC_NOWRITE;

    if (diskless)
      flags |= NC_DISKLESS;
  } else if (mode == "r+" || mode == "a") {
    flags = NC_WRITE;

    if (diskless)
      flags |= NC_DISKLESS;
  } else if (mode == "as" || mode == "r+s") {
    flags = NC_SHARE;

    if (diskless)
      flags |= NC_DISKLESS;
  } else {
    throw std::invalid_argument("mode must be 'w', 'r', 'a', 'r+', 'ws', "
        "'as' or 'r+s' got '" + mode + "'");
  }

  Check(mode[0] == 'w'
      ? nc_create(filename.c_str(), flags, &ident)
      : nc_open(filename.c_str(), flags, &ident));

  nc_id_ = ident;
}

void File::SetRedefineMode(bool redefine_mode) const {
  if (redefine_mode) {
    int status = nc_redef(nc_id_);
    if (status != NC_NOERR && status != NC_EINDEFINE)
      throw std::runtime_error(nc_strerror(status));
  } else {
    Check(nc_enddef(nc_id_));
  }
}

}  // namespace netcdf
