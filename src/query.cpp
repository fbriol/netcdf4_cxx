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

#include <netcdf4_cxx/parser.hpp>
#include <netcdf4_cxx/query.hpp>

namespace netcdf {

std::valarray<double> Query::Evaluate(const File& file,
                                      const std::string& query,
                                      const std::string& unit) {
  parser::LiteralExpression expr(QueryProxy(*this, file, unit), query);
  Any result = expr.Evaluate();
  if (result.IsTyped(typeid(double)))
    return std::valarray<double>{result.Cast<double>()};
  return result;
}

}  // namespace netcdf