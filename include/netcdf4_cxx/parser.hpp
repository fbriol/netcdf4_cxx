#pragma once
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include "any.hpp"
#include "query.hpp"

namespace netcdf {
namespace parser {

/**
 * Know token types
 */
enum class Kind {
  kFunction,                //!< kFunction
  kName,                    //!< kName
  kVariable,                //!< kVariable
  kNumber,                  //!< kNumber
  kEnd,                     //!< kEnd
  kEquals,                  //!< kEquals
  kNotEquals,               //!< kNotEquals
  kLessThan,                //!< kLessThan
  kGreaterThan,             //!< kGreaterThan
  kLessThanOrEqualTo,       //!< kLessThanOrEqualTo
  kGreaterThanOrEqualTo,    //!< kGreaterThanOrEqualTo
  kAnd,                     //!< kAnd
  kOr,                      //!< kOr
  kPlus = '+',              //!< kPlus
  kMinus = '-',             //!< kMinus
  kMul = '*',               //!< kMul
  kDiv = '/',               //!< kDiv
  kModulo = '%',            //!< kModulo
  kEnds = ';',              //!< kEnds
  kAssign = '=',            //!< kAssign
  kLeftParenthesis = '(',   //!< kLeftParenthesis
  kRightParenthesis = ')',  //!< kRightParenthesis
  kLeftAccolade = '{',      //!< kLeftAccolade
  kRightAccolade = '}',     //!< kRightAccolade
  kComma = ','              //!< kComma
};

/**
 * Handle the stream of the characters to be parsed.
 */
class TokenStream {
 private:
  bool full_;
  Kind kind_;
  std::stringstream stream_;
  Any value_;

 public:
  /**
   * Default constructor
   * @param s string to parse
   */
  TokenStream(const std::string& s)
      : full_(false), kind_(Kind::kEnd), stream_(s), value_() {}

  /**
   * Gets the value parsed
   * @return the value parsed
   */
  Any& value() noexcept { return value_; }

  /**
   * Return the stream pointer to the beginning of the string to be parsed.
   */
  void Reset() { stream_.seekg(0, stream_.beg); }

  /**
   * Determine if the analysis is over.
   *
   * @return true if the analysis of the string is complete.
   */
  operator bool() { return !stream_.eof(); }

  /// Replace the last valid token in the stream.
  void PutBack(const Kind kind) {
    if (full_) throw std::logic_error("reset input stream into a full buffer");
    kind_ = kind;
    full_ = true;
  }

  /**
   * Get the string that is being parsed.
   *
   * @return the string
   */
  std::string ToString() {
    return std::string(stream_.str().substr(0, stream_.tellg()));
  }

  /**
   * Get the next valid token found
   *
   * @return the valid token found
   */
  const Kind& Get();
};

/**
 * Exception thrown when the parser encountered a syntax error
 */
class SyntaxError : public std::runtime_error {
 public:
  /**
   * Constructs the exception object with msg as explanatory string and
   * the information of the parsed string.
   *
   * @param msg explanatory string
   * @param stream properties of the parsed string
   */
  SyntaxError(const std::string& msg, TokenStream& stream)
      : std::runtime_error(msg + ": " + stream.ToString() + "<-- here") {}
};

/**
 * Type of known identifiers
 */
enum class IdentifierType {
  kNotAFunction,  //!< kNotAFunction
  kConstant,      //!< kConstant
  kUnary,         //!< kUnary
  kBinary,        //!< kBinary
  kTernary        //!< kTernary
};

/**
 * Evaluate a literal expression
 *
 * @verbatim
 *  Statement:
 *      Assignment
 *      Or
 *  Assignment:
 *      Name = Or
 *  Or:
 *      And
 *      Or || And
 *  And:
 *      Equality
 *      And && Equality
 *  Equality:
 *      Comparison
 *      Equality == Comparison
 *      Equality != Comparison
 *  Comparison:
 *      Expression
 *      Comparison >= Expression
 *      Comparison >  Expression
 *      Comparison <  Expression
 *      Comparison <= Expression
 *  Expression:
 *      Term
 *      Expression + Term
 *      Expression - Term
 *  Term:
 *      Primary
 *      Term * Primary
 *      Term / Primary
 *      Term % Primary
 *  Primary:
 *      Number
 *      ${Name}
 *      Name
 *      Name = Or
 *      ( Or )
 *      - Or
 *      + Or
 *  Number:
 *      floating-point-literal
 *  Name:
 *      [a-zA-Z][a-zA-Z_0-9]*
 * @endverbatim
 */
class LiteralExpression {
 public:
  /**
   * Default constructor
   *
   * @param string string to parse
   */
  LiteralExpression(const QueryProxy& query, const std::string& string)
      : query_(query), stream_(string), variable_() {}

  /**
   * Evaluate the expression
   *
   * @return the result of the literal expression
   */
  Any Evaluate() {
    Any result;
    stream_.Reset();
    while (stream_) {
      Kind token = Kind::kEnd;
      while ((token = stream_.Get()) == Kind::kEnds)
        ;
      if (token == Kind::kEnd) break;
      stream_.PutBack(token);
      result = Or();
    }
    return result;
  }

 private:
  const QueryProxy& query_;
  TokenStream stream_;
  std::map<std::string, Any> variable_;

  static std::map<std::string, double> constant_;
  static std::map<std::string, std::function<Any(Any)>> unary_;
  static std::map<std::string, std::function<Any(Any, Any)>> binary_;
  static std::map<std::string, std::function<Any(Any, Any, Any)>> ternary_;

  // Store the content of a variable
  void SetValue(const std::string& name, const Any& value) {
    variable_.insert(std::pair<std::string, Any>(name, value));
  }

  // Get the type of function associated with an identifier.
  IdentifierType GetIdentifierType(const std::string& identifier) const {
    if (unary_.count(identifier) != 0)
      return IdentifierType::kUnary;
    else if (binary_.count(identifier) != 0)
      return IdentifierType::kBinary;
    else if (ternary_.count(identifier) != 0)
      return IdentifierType::kTernary;
    else if (constant_.count(identifier) != 0)
      return IdentifierType::kConstant;
    return IdentifierType::kNotAFunction;
  }

  // Handle the action associated with an identifier.
  Any HandleIdentifier(const std::string& identifier);

  // Call a function
  Any Call(const std::string& identifier, const IdentifierType identifier_type);

  // Get the value of a variable
  Any& GetValue(const std::string& name) {
    auto it = variable_.find(name);
    if (it == variable_.end())
      throw std::runtime_error("name '" + name + "' is not defined");
    return it->second;
  }

  // Handle +value or -value
  static Any& Unary(const double factor, Any&& value) {
    return factor < 0 ? -value : +value;
  }

  // Handle binary operators +, -, %, *, /
  Any& BinaryOperator(const Kind kind, Any&& left, Any&& right);

  // Load a NetCDF variable
  Any LoadVariable();

  // Grammar functions
  Any Primary();
  Any Term();
  Any Expression();
  Any Comparison();
  Any Equality();
  Any And();
  Any Or();
  Any Statement();
};
}  // namespace parser
}  // namespace netcdf