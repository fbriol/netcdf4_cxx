#pragma once

#include <netcdf4_cxx/file.hpp>
#include <netcdf4_cxx/units.hpp>
#include <string>

namespace netcdf {

/**
 * Execute queries on NetCDF files.
 *
 * Queries are mathematical expression using NetCDF variables as arguments. The
 * parser has the fallowing capatibilities
 *  * Mathematical operators (+, -, *, /, %)
 *  * Functions (abs, exp, log, log10, sqrt, pow, iif)
 *  * Trigonometry (sin, cos, tan, asin, acos, atan, sinh, cosh, tanh, atan2)
 *  * Equalities, Inequalities(==, !=, <, <=, >, >=)
 *  * Boolean logic (&&, ||, &, |)
 *  * Constants (e, log2e, log10e, ln2, ln10, pi, pi_2, pi_4, 1_pi, 2_pi,
 *               2_sqrtpi, sqrt2, sqrt1_2)
 *  * Expression local variables
 *  * Expression NetCDF variable : ${X} where X is the NetCDF variable name
 *    Expressions can handle physical units of NetCDF variables.
 *
 *  Example:
 *  @code
 *    X = ${X} * 0.001; Y = abs(${Y}); sqrt(X/Y)
 *  @endcode
 */
class Query {
 private:
  units::Parser parser_;

 public:
  /**
   * Default constructor
   *
   * @param path path to the udunits2 XML database to be used
   */
  Query(const std::string& path = "") : parser_(path) {}

  /**
   * Evaluate the mathematical expression on the NetCDF file.
   *
   * @param file NetCDF File to be query
   * @param query mathematical expression
   * @param unit unit of result
   * @return the result of the expression
   */
  std::valarray<double> Evaluate(const File& file, const std::string& query,
                                 const std::string& unit = "");

  /**
   * Conversion of values from one physical unit to another.
   *
   * @param from the unit from which to convert values.
   * @param to the unit to which to convert values.
   * @return the values converted
   */
  std::valarray<double>& ConvertToSamePysicalUnit(
      const std::string& from, const std::string& to,
      std::valarray<double>& values) const {
    auto converter = parser_.Parse(from, to);
    return converter.Convert(values);
  }
};

/**
 * Provides a modified interface to Query
 */
class QueryProxy {
 private:
  const File& file_;
  const Query& query_;
  const std::string& unit_;

 public:
  /**
   * Default constructor
   *
   * @param query Query instance to be used
   * @param file NetCDF file to be used
   * @param unit unit of the result of the query
   */
  QueryProxy(const Query& query, const File& file, const std::string& unit)
      : query_(query), file_(file), unit_(unit) {}

  /**
   * Load a variable from the NetCDF File handled
   *
   * @param name Variable name
   * @result the values read
   */
  std::valarray<double> LoadVariable(const std::string& name) const {
    auto variable = file_.FindVariable(name);
    if (!variable) throw std::runtime_error(name + ": no such variable");
    std::valarray<double> values = variable->ReadMaskAndScale<double>();
    if (!unit_.empty()) {
      auto units = variable->FindAttribute("units");
      query_.ConvertToSamePysicalUnit(unit_, units ? units->ReadText() : "1",
                                      values);
    }
    return values;
  }
};

}  // namespace netcdf
