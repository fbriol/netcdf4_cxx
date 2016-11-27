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

#pragma once

#include <netcdf4_cxx/attribute.hpp>
#include <netcdf4_cxx/dataset.hpp>
#include <netcdf4_cxx/group.hpp>
#include <netcdf4_cxx/netcdf.hpp>
#include <netcdf.h>
#include <stddef.h>
#include <cstdio>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>

namespace netcdf {

/**
 * Binary storage format
 */
enum class Format {
  /*! Files use the version 4 disk format (HDF5) and use the new
   * features of the version 4 API */
  kNetCdf4 = NC_FORMAT_NETCDF4,
  /*! Files use the version 4 disk format (HDF5), but do not use any
   * features not found in the version 3 API. They can be read by
   * netCDF 3 clients only if they have been relinked against the netCDF
   * 4 library. They can also be read by HDF5 clients. */
  kClassicNetCdf4 = NC_FORMAT_NETCDF4_CLASSIC,
  /*! Files use the original netCDF file format. */
  kClassicNetCdf3 = NC_FORMAT_CLASSIC,
  /*! Files use the original netCDF file format with 64-bit offset
   * format; relaxes limitations on creating very large files */
  k64BitsNetCdf3 = NC_FORMAT_64BIT
};

/**
 * A netCDF File is a collection of dimensions, groups, variables and
 * attributes. Together they describe the meaning of data and relations
 * among data fields stored in a netCDF file
 */
class File : public Group {
 public:
  /**
   * Constructor
   */
  File()
      : Group() {
  }

  /**
   * Constructor
   *
   * @param filename Name of netCDF file to hold dataset.
   * @param mode access mode. r means read-only; no data can be modified.
   *  w means write; a new file is created, an existing file with the same
   *  name is deleted. a and r+ mean append (in analogy with serial files);
   *  an existing file is opened for reading and writing. Appending s to modes
   *  w, r+ or a will enable unbuffered shared access to #kClassicNetCdf3 or
   *  #k64BitsNetCdf3 formatted files. Unbuffered acesss may be useful even if
   *  you don't need shared access, since it may be faster for programs that
   *  don't access data sequentially. This option is ignored for #kNetCdf4 and
   *  #kClassicNetCdf4 formatted files
   * @param clobber if True (default), opening a file with mode='w' will
   *  clobber an existing file with the same name. if False, an exception
   *  will be raised if a file with the same name already exists.
   * @param diskless If True, create diskless (in memory) file.
   * @param persist if diskless=True, persist file to disk when closed
   *  (default False).
   * @param format underlying file format
   */
  File(const std::string& filename,
       const std::string& mode = "r",
       bool clobber = true,
       const bool diskless = false,
       const bool persist = false,
       const Format format = Format::kNetCdf4)
      : Group() {
    Open(filename, mode, clobber, diskless, persist, format);
  }

  /**
   * Open or create a netCDF file
   *
   * @param filename name of the netCDF file to hold dataset
   * @param mode access mode
   * @param clobber if true overwrite existing file
   * @param diskless if true, create diskless (memory) file
   * @param persist if diskless=True, persist file to disk when closed
   * @param format underlying file format
   *
   * @sa #File
   */
  void Open(const std::string& filename,
            const std::string& mode = "r",
            bool clobber = true,
            const bool diskless = false,
            const bool persist = false,
            const Format format = Format::kNetCdf4);

  /**
   * Destructor
   */
  virtual ~File() {
    nc_close(nc_id_);
  }

  /**
   * Close the file
   */
  inline void Close() {
    Check(nc_close(nc_id_));
  }

  /**
   * Set the redefine mode
   *
   * @param redefine_mode start or end define mode
   */
  void SetRedefineMode(const bool redefine_mode) const;

  /**
   * Synchronize the Dataset to Disk
   */
  inline void Synchronize() const {
    Check(nc_sync(nc_id_));
  }

  /**
   * Change the default chunk cache settings in the HDF5 library for all
   * variables in the file
   */
  inline void SetChunkCache(const size_t size, const size_t items,
                            const float preemption) const {
    Check(nc_set_chunk_cache(size, items, preemption));
  }

  /**
   * Get the chunk cache settings for the HDF5 library
   */
  inline void GetChunkCache(size_t& size, size_t& items,
                            float& preemption) const {
    Check(nc_get_chunk_cache(&size, &items, &preemption));
  }

  /**
   * Get the NetCDF library version used
   *
   * @return the NetCDF library version
   */
  static std::string GetVersion() {
    return nc_inq_libvers();
  }

  /**
   * Get the file system path used to open/create the NetCDF file;
   *
   * @return the file path
   */
  std::string GetFilePath() const {
    char result[FILENAME_MAX + 1];
    Check(nc_inq_path(nc_id_, nullptr, result));
    return result;
  }

  /**
   * Get the binary format of the netCDF file
   *
   * @return the binary format of the file
   */
  Format GetFormat() const {
    int result;
    Check(nc_inq_format(nc_id_, &result));
    return static_cast<Format>(result);
  }

  /**
   * Put open NetCDF dataset into define mode.
   *
   * For netCDF-4 files (i.e. files created with File::kNetCdf4 in the mode
   * in their call to Open()), it is not necessary to call this method.
   */
  void EnterDefineMode() const {
    Check(nc_redef(nc_id_));
  }

  /**
   * Leave define mode
   *
   * For netCDF-4 files (i.e. files created with File::kNetCdf4 in the mode
   * in their call to Open()), it is not necessary to call this method.
   */
  void LeaveDefineMode() const {
    Check(nc_enddef(nc_id_));
  }

  /**
   * Get the human-readable title, if it exists.
   *
   * @return title or an empty string if the title of the file is not defined
   */
  std::string GetTitle() const {
    auto result = FindAttribute("title");
    if (result != nullptr)
      return result->ReadText();
    return std::string();
  }
};

}  // namespace netcdf
