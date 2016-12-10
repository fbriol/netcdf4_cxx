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
#include <valarray>

namespace netcdf {
namespace parser {

std::map<std::string, double> LiteralExpression::constant_ = {
    {"e", M_E},
    {"log2e", M_LOG2E},
    {"log10e", M_LOG10E},
    {"ln2", M_LN2},
    {"ln10", M_LN10},
    {"pi", M_PI},
    {"pi_2", M_PI_2},
    {"pi_4", M_PI_4},
    {"1_pi", M_1_PI},
    {"2_pi", M_2_PI},
    {"2_sqrtpi", M_2_SQRTPI},
    {"sqrt2", M_SQRT2},
    {"sqrt1_2", M_SQRT1_2}};

std::map<std::string, std::function<Any(Any)>> LiteralExpression::unary_ = {
    {"abs", Any::abs},     {"exp", Any::exp},   {"log", Any::log},
    {"log10", Any::log10}, {"sqrt", Any::sqrt}, {"sin", Any::sin},
    {"cos", Any::cos},     {"tan", Any::tan},   {"asin", Any::asin},
    {"acos", Any::acos},   {"atan", Any::atan}, {"sinh", Any::sinh},
    {"cosh", Any::cosh},   {"tanh", Any::tanh}};

std::map<std::string, std::function<Any(Any, Any)>> LiteralExpression::binary_ =
    {{"pow", Any::pow}, {"atan2", Any::atan2}};

std::map<std::string, std::function<Any(Any, Any, Any)>>
    LiteralExpression::ternary_ = {{"iif", Any::iif}};

const Kind& TokenStream::Get() {
  char current, next;

  if (full_) {
    full_ = false;
    return kind_;
  }

  // skip whitespace
  do {
    if (!stream_.get(current)) return kind_ = Kind::kEnd;
  } while (std::isspace(current));

  switch (current) {
    case ';':
    case '*':
    case '/':
    case '%':
    case '+':
    case '-':
    case '(':
    case ')':
    case '{':
    case '}':
      return kind_ = static_cast<Kind>(current);
    // = or ==
    case '=':
      stream_.get(next);
      if (next == '=') return kind_ = Kind::kEquals;
      stream_.unget();
      return kind_ = Kind::kAssign;
    case '$':
      return kind_ = Kind::kVariable;
    case ',':
      return kind_ = Kind::kComma;
    // Number
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
      next = stream_.peek();
      if (next != '_') {
        stream_.unget();
        double number;
        stream_ >> number;
        value_ = number;
        return kind_ = Kind::kNumber;
      }
    default:
      break;
  }

  // & or &&
  if (current == '&') {
    stream_.get(next);
    if (next == '&') return kind_ = Kind::kAnd;
    stream_.unget();
  }

  // | or ||
  if (current == '|') {
    stream_.get(next);
    if (next == '|') return kind_ = Kind::kOr;
    stream_.unget();
  }

  // !=
  if (current == '!') {
    stream_.get(next);
    if (next == '=') return kind_ = Kind::kNotEquals;
    stream_.unget();
  }

  // > or >=
  if (current == '>') {
    stream_.get(next);
    if (next == '=') return kind_ = Kind::kGreaterThanOrEqualTo;
    return kind_ = Kind::kGreaterThan;
  }

  // < or <=
  if (current == '<') {
    stream_.get(next);
    if (next == '=') return kind_ = Kind::kLessThanOrEqualTo;
    return kind_ = Kind::kLessThan;
  }

  if (isalnum(current)) {
    std::string name(1, current);
    while (stream_.get(current) &&
           (isalpha(current) || isdigit(current) || current == '_'))
      name += current;
    stream_.unget();
    value_ = name;
    return kind_ = Kind::kName;
  }
  throw SyntaxError("bad token: ", *this);
}

Any LiteralExpression::Call(const std::string& identifier,
                            const IdentifierType function_type) {
  Any result, args[3];

  if (stream_.Get() != Kind::kLeftParenthesis)
    throw SyntaxError("'(' expected", stream_);

  switch (function_type) {
    case IdentifierType::kTernary:
      args[0] = Or();
      if (stream_.Get() != Kind::kComma)
        throw SyntaxError("',' expected", stream_);
    case IdentifierType::kBinary:
      args[1] = Or();
      if (stream_.Get() != Kind::kComma)
        throw SyntaxError("',' expected", stream_);
    case IdentifierType::kUnary:
      args[2] = Or();
    default:
      break;
  }

  if (stream_.Get() != Kind::kRightParenthesis)
    throw SyntaxError("')' expected", stream_);

  switch (function_type) {
    case IdentifierType::kTernary:
      return ternary_[identifier](args[0], args[1], args[2]);
    case IdentifierType::kBinary:
      return binary_[identifier](args[1], args[2]);
    default:
      return unary_[identifier](args[2]);
  }
}

Any LiteralExpression::HandleIdentifier(const std::string& identifier) {
  const IdentifierType identifier_type = GetIdentifierType(identifier);

  // Identifier is a constant
  if (identifier_type == IdentifierType::kConstant)
    return constant_[identifier];

  // Identifier is unknown
  if (identifier_type == IdentifierType::kNotAFunction) {
    Kind token = stream_.Get();
    if (token == Kind::kAssign)
      SetValue(identifier, Or());
    else
      stream_.PutBack(token);
    return GetValue(identifier);
  }

  // Identifier is a function
  return Call(identifier, identifier_type);
}

Any LiteralExpression::LoadVariable() {
  Kind token = stream_.Get();
  if (token != Kind::kLeftAccolade) throw SyntaxError("'{' expected", stream_);
  token = stream_.Get();
  if (token != Kind::kName) throw SyntaxError("identifier expected", stream_);
  std::string identifier = stream_.value();
  token = stream_.Get();
  if (token != Kind::kRightAccolade) throw SyntaxError("'}' expected", stream_);
  return query_.LoadVariable(identifier);
}

Any LiteralExpression::Primary() {
  Any result;
  Kind token = stream_.Get();
  switch (token) {
    // handle '(' or ')'
    case Kind::kLeftParenthesis: {
      Any value = Or();
      if (stream_.Get() != Kind::kRightParenthesis)
        throw SyntaxError("')' expected", stream_);
      return value;
    }

    // handle a number value
    case Kind::kNumber:
      return stream_.value();

    // handle a variable
    case Kind::kVariable:
      return LoadVariable();
      break;

    // handle a name: variable, function
    case Kind::kName:
      return HandleIdentifier(stream_.value());
      break;

    // handle -Or()
    case Kind::kMinus:
      return Unary(-1, Or());

    // handle +Or()
    case Kind::kPlus:
      return Unary(1, Or());
    default:
      break;
  }
  throw SyntaxError("primary expected", stream_);
}

Any& LiteralExpression::BinaryOperator(const Kind kind, Any&& left,
                                       Any&& right) {
  // Operation on scalars
  switch (kind) {
    case Kind::kPlus:
      left += right;
      return left;
      break;
    case Kind::kMinus:
      left -= right;
      return left;
      break;
    case Kind::kMul:
      left *= right;
      return left;
      break;
    case Kind::kDiv:
      left /= right;
      return left;
      break;
    case Kind::kModulo:
      left %= right;
      return left;
      break;
    default:
      throw SyntaxError("unknown binary operator", stream_);
      break;
  }
}

Any LiteralExpression::Term() {
  Any left = Primary();
  Kind token = stream_.Get();

  while (true) {
    switch (token) {
      case Kind::kMul:
        left = BinaryOperator(Kind::kMul, std::move(left), Primary());
        token = stream_.Get();
        break;
      case Kind::kDiv:
        left = BinaryOperator(Kind::kDiv, std::move(left), Primary());
        token = stream_.Get();
        break;
      case Kind::kModulo:
        left = BinaryOperator(Kind::kModulo, std::move(left), Primary());
        token = stream_.Get();
        break;
      default:
        stream_.PutBack(token);
        return left;
    }
  }
}

Any LiteralExpression::Expression() {
  Any left = Term();
  Kind token = stream_.Get();

  while (true) {
    switch (token) {
      case Kind::kPlus:
        left = BinaryOperator(Kind::kPlus, std::move(left), Term());
        token = stream_.Get();
        break;
      case Kind::kMinus:
        left = BinaryOperator(Kind::kMinus, std::move(left), Term());
        token = stream_.Get();
        break;
      default:
        stream_.PutBack(token);
        return left;
    }
  }
}

Any LiteralExpression::Comparison() {
  Any left = Expression();
  Kind token = stream_.Get();

  while (true) {
    switch (token) {
      case Kind::kGreaterThanOrEqualTo:
        left = left >= Expression();
        token = stream_.Get();
        break;
      case Kind::kGreaterThan:
        left = left > Expression();
        token = stream_.Get();
        break;
      case Kind::kLessThan:
        left = left < Expression();
        token = stream_.Get();
        break;
      case Kind::kLessThanOrEqualTo:
        left = left <= Expression();
        token = stream_.Get();
        break;
      default:
        stream_.PutBack(token);
        return left;
    }
  }
}

Any LiteralExpression::Equality() {
  Any left = Comparison();
  Kind token = stream_.Get();

  while (true) {
    switch (token) {
      case Kind::kEquals:
        left = left == Comparison();
        token = stream_.Get();
        break;
      case Kind::kNotEquals:
        left = left != Comparison();
        token = stream_.Get();
        break;
      default:
        stream_.PutBack(token);
        return left;
    }
  }
}

Any LiteralExpression::And() {
  Any left = Equality();
  Kind token = stream_.Get();

  while (true) {
    switch (token) {
      case Kind::kAnd:
        left = left && Equality();
        token = stream_.Get();
        break;
      default:
        stream_.PutBack(token);
        return left;
    }
  }
}

Any LiteralExpression::Or() {
  Any left = And();
  Kind token = stream_.Get();

  while (true) {
    switch (token) {
      case Kind::kOr:
        left = left || And();
        token = stream_.Get();
        break;
      default:
        stream_.PutBack(token);
        return left;
    }
  }
}

}  // namespace parser
}  // namespace netcdf