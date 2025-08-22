#pragma once

#include <numbers>
#include <string_view>
#include <variant>

namespace cnmea::types {

enum class Units {
  ms,
  kmh,
  knots,
};

enum class Direction {
  North,
  South,
  East,
  West,
};

constexpr double KNTOMS{0.514444444};

constexpr double KNTOKMH{1.85};

enum class ParseError {
  InvalidDirection,
  InvalidFormat,
  MissingFields,
  UnknownError,
  UnsupportedType,
  InvalidLatitude,
  InvalidLongitude,
  InvalidSpeed,
  InvalidCourse,
  InvalidUTCDate,
  InvalidUTCTime,
  InvalidMagneticVariation,
  InvalidMode
};

struct Course {
private:
  double degrees;

public:
  Course(double degrees) : degrees(degrees) {}
  double value_degrees() const { return degrees; }
  double value_radians() const {
    return value_degrees() * std::numbers::pi / 180.0;
  }
};

struct Latitude {
private:
  double degrees;
  Direction direction;

public:
  Latitude(double degrees, Direction direction)
      : degrees(degrees), direction(direction) {}
  double get_degrees() const { return degrees; }
  Direction get_direction() const { return direction; }
  double value_degrees() const {
    return (direction == Direction::North ? degrees : -degrees);
  }
  double value_radians() const {
    return value_degrees() * std::numbers::pi / 180.0;
  }
};

struct Longitude {
private:
  double degrees;
  Direction direction;

public:
  Longitude(double degrees, Direction direction)
      : degrees(degrees), direction(direction) {}
  double get_degrees() const { return degrees; }
  Direction get_direction() const { return direction; }
  double value_degrees() const {
    return (direction == Direction::East ? degrees : -degrees);
  }
  double value_radians() const {
    return value_degrees() * std::numbers::pi / 180.0;
  }
};

struct Speed {
private:
  double value;
  Units units = Units::knots;

public:
  Speed(double value, Units units = Units::knots)
      : value(value), units(units) {}
  double get_value() const { return value; }
  Units get_units() const { return units; }
};

struct UTCTime {
  std::string_view hours;
  std::string_view minutes;
  std::string_view seconds;
};

struct UTCDate {
  std::string_view day;
  std::string_view month;
  std::string_view year;
};

enum class Type {
  GGA,
  RMC,
  VTG,
};

enum class Status {
  Active,
  Void,
};

enum class Mode {
  Autonomous,
  Differential,
  Estimated,
  NotValid,
};

struct MagneticVariation {
private:
  double degrees;
  Direction direction;

public:
  MagneticVariation(double degrees, Direction direction)
      : degrees(degrees), direction(direction) {}
  double get_degrees() const { return degrees; }
  Direction get_direction() const { return direction; }
  double value_degrees() const {
    return (direction == Direction::East ? degrees : -degrees);
  }
  double value_radians() const {
    return value_degrees() * std::numbers::pi / 180.0;
  }
};

using Element = std::variant<types::ParseError>;

} // namespace cnmea::types
