#pragma once

#include <numbers>
#include <string_view>
#include <variant>

namespace cnmea::types {

enum class SpeedUnits {
  ms,
  kmh,
  knots,
};

enum class DistanceUnits {
  m,
  km,
  ft,
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
  SpeedUnits units = SpeedUnits::knots;

public:
  Speed(double value, SpeedUnits units = SpeedUnits::knots)
      : value(value), units(units) {}
  double get_value() const { return value; }
  SpeedUnits get_units() const { return units; }
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
  GLL,
  VTG,
  ZDA,
};

enum class Status {
  Valid,
  Invalid,
};

enum class Mode {
  Autonomous,
  Differential,
  Estimated,
  ManualInput,
  Simulation,
  NotValid,
  Precise,
  RTKFixed,
  RTKFloat,
  Uncalibrated,
};

enum class FixQuality {
  Invalid = 0,
  GPS = 1,
  DGPS = 2,
  PPS = 3,
  RealTimeKinematic = 4,
  FloatRTK = 5,
  Estimated = 6,
  ManualInput = 7,
  Simulation = 8
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
struct Altitude {
private:
  double value;
  DistanceUnits units;

public:
  Altitude(double value, DistanceUnits units = DistanceUnits::m)
      : value(value), units(units) {}
  double get_value() const { return value; }
  DistanceUnits get_units() const { return units; }
  double value_meters() const { return value; }
  double value_feet() const { return value * 3.28084; }
};

struct GeoidSeparation {
private:
  double value;
  DistanceUnits units;

public:
  GeoidSeparation(double value, DistanceUnits units = DistanceUnits::m)
      : value(value), units(units) {}
  double get_value() const { return value; }
  DistanceUnits get_units() const { return units; }
  double value_meters() const { return value; }
  double value_feet() const { return value * 3.28084; }
};

struct AgeOfDgps {
private:
  double seconds;

public:
  AgeOfDgps(double seconds) : seconds(seconds) {}
  double value_seconds() const { return seconds; }
  double value_minutes() const { return seconds / 60.0; }
};

struct DgpsStationId {
private:
  int id;

public:
  DgpsStationId(int id) : id(id) {}
  int value() const { return id; }
};

using Element = std::variant<types::ParseError>;

} // namespace cnmea::types
