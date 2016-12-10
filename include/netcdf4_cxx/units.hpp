#pragma once

#include <memory>
#include <string>
#include <system_error>

struct ut_system;

namespace netcdf {
namespace units {

/**
 * @brief Exceptions raised by udunits2.
 */
class Error : public std::runtime_error {
 public:
  /**
   * @brief Default Constructor
   */
  Error();

  /**
   * @brief Constructs a new instance with a character string describing the
   * error raised by uduntis2
   *
   * @param msg a character string describing the error
   */
  explicit Error(const std::string& msg) : std::runtime_error(msg) {}
};

/**
 * @brief Error raised during the initialization of udunits2
 */
class SystemError : public std::system_error {
 public:
  /**
   * @brief Default Constructor
   */
  explicit SystemError(const std::error_code& error_code = std::error_code())
      : std::system_error(error_code) {}

  /**
   * @brief Constructs a new instance with a character string describing the
   * error raised by uduntis2
   *
   * @param error_code platform-dependent error code
   * @param what a character string describing the error
   */
  SystemError(const std::error_code& error_code, const std::string& what)
      : std::system_error(error_code, what) {}
};

/**
 * @brief Handles a unit converter
 */
class Converter {
 public:
  /**
   * Default constructor
   *
   * @param offset the numeric offset
   * @param scale the numeric scale factor
   */
  explicit Converter(const double offset = 0, const double scale = 1)
      : offset_(offset), scale_(scale) {}

  /**
   * Tests if the converter is null
   *
   * @return true if the converter is null otherwise false
   */
  inline bool IsNull() const noexcept { return offset_ == 0 && scale_ == 1; }

  /**
   * Converts a value in one unit to equivalent value in another
   *
   * @param value value to convert
   */
  template <class T>
  inline constexpr T& Convert(T& value) const {
    if (!IsNull()) {
      value *= scale_;
      value += offset_;
    }
    return value;
  }

  /**
   * Converts a value in one unit to equivalent value in another
   *
   * @param value value to convert
   */
  template <class T>
  inline constexpr T Convert(const T& value) const {
    return IsNull() ? value : value * scale_ + offset_;
  }

  /**
   * @return A string representation of the converter
   */
  std::string ToString() const {
    return std::string("x * ") + std::to_string(scale_) + " + " +
           std::to_string(offset_);
  }

  /**
   * @return the numeric factor used by the converter
   */
  double scale() const noexcept { return scale_; }

  /**
   * @return the particular origin used by the converter
   */
  double offset() const noexcept { return offset_; }

 private:
  double offset_;
  double scale_;
};

/**
 * Provides support for units of physical quantities.
 */
class Parser {
 public:
  /**
   * Default constructor
   *
   * Loads the XML-formatted unit-database. The pathname specified by the
   * environment variable UDUNITS2_XML_PATH is used if set; otherwise, the
   * compile-time pathname of the installed, default, unit database is used.
   */
  Parser() : system_() { Initialize(nullptr); }

  /**
   * Loads the XML-formatted unit-database specified by path
   *
   * @param path path to the XML database to used
   */
  explicit Parser(const std::string& path) : system_() {
    Initialize(path.empty() ? nullptr : path.c_str());
  }

  /**
   * @brief Frees resources used by udunits2
   */
  virtual ~Parser() {}

  /**
   * @brief Computes a converter of numeric values in unit "from" to numeric
   * values in unit "to".
   *
   * @param from the unit from which to convert values.
   * @param to the unit to which to convert values.
   * @return The converter computed
   */
  Converter Parse(const std::string& from, const std::string& to) const;

  /**
   * @brief Checks if numeric values in unit "from" are convertible to numeric
   * values in unit "to"
   *
   * @param unit1 a unit.
   * @param unit2 another unit.
   *
   * @return Numeric values can be converted between the units.
   */
  bool AreConvertible(const std::string& unit1, const std::string& unit2) const;

 private:
  std::shared_ptr<ut_system> system_;

  // Reads the XML-formatted unit-database
  void Initialize(const char* path = nullptr);
};

}  // namespace units
}  // namespace netcdf
