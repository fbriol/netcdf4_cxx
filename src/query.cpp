#include <netcdf4_cxx/parser.hpp>
#include <netcdf4_cxx/query.hpp>

namespace netcdf {

std::valarray<double> Query::Evaluate(const File& file,
                                      const std::string& query,
                                      const std::string& unit) {
  parser::LiteralExpression expr(QueryProxy(*this, file, unit), query);
  return expr.Evaluate();
}

}  // namespace netcdf