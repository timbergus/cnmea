#pragma once

#include <format>
#include <string>

#include "types.h"

namespace cnmea::p_tools {

inline std::string to_string(const types::Status &status) {
  switch (status) {
  case types::Status::Active:
    return "Active";
  case types::Status::Void:
    return "Void";
  }
  return "--";
}

inline std::string to_string(const std::optional<types::Mode> &mode) {
  if (mode) {
    switch (mode.value()) {
    case types::Mode::Autonomous:
      return "Autonomous";
    case types::Mode::Differential:
      return "Differential";
    case types::Mode::Estimated:
      return "Estimated";
    case types::Mode::NotValid:
      return "Not Valid";
    }
  }
  return "--";
}

inline std::string to_string(const types::Units &units) {
  switch (units) {
  case types::Units::ms:
    return "m/s";
  case types::Units::kmh:
    return "km/h";
  case types::Units::knots:
    return "knots";
  }
  return "--";
}

inline std::string to_string(const types::Direction &direction) {
  switch (direction) {
  case types::Direction::North:
    return "North";
  case types::Direction::South:
    return "South";
  case types::Direction::East:
    return "East";
  case types::Direction::West:
    return "West";
  }
  return "--";
}

inline std::string to_string(const std::optional<types::Latitude> &latitude) {
  if (latitude) {
    return std::format("{} {}", latitude->get_degrees(),
                       to_string(latitude->get_direction()));
  }
  return "--";
}

inline std::string to_string(const std::optional<types::Longitude> &longitude) {
  if (longitude) {
    return std::format("{} {}", longitude->get_degrees(),
                       to_string(longitude->get_direction()));
  }
  return "--";
}

inline std::string to_string(const std::optional<types::Course> &course) {
  if (course) {
    return std::format("{}", course->value_degrees());
  }
  return "--";
}

inline std::string to_string(const std::optional<types::UTCDate> &utc_date) {
  if (utc_date) {
    return std::format("{}/{}/{}", utc_date->day, utc_date->month,
                       utc_date->year);
  }
  return "--";
}

inline std::string to_string(const types::UTCTime &utc_time) {
  return std::format("{}:{}:{}", utc_time.hours, utc_time.minutes,
                     utc_time.seconds);
}

inline std::string to_string(const std::optional<types::Speed> &speed) {
  if (speed) {
    return std::format("{} {}", speed->get_value(),
                       to_string(speed->get_units()));
  }
  return "--";
}

inline std::string
to_string(const std::optional<types::MagneticVariation> &magnetic_variation) {
  if (magnetic_variation) {
    return std::format("{} {}", magnetic_variation->value_degrees(),
                       to_string(magnetic_variation->get_direction()));
  }
  return "--";
}

inline std::string to_string(const types::Type &type) {
  switch (type) {
  case types::Type::GGA:
    return "GGA";
  case types::Type::RMC:
    return "RMC";
  case types::Type::VTG:
    return "VTG";
  }
  return "--";
}

inline std::string to_string(const types::ParseError &error) {
  switch (error) {
  case types::ParseError::InvalidFormat:
    return "Invalid Format";
  case types::ParseError::UnsupportedType:
    return "Unsupported Type";
  case types::ParseError::InvalidDirection:
    return "Invalid Direction";
  case types::ParseError::MissingFields:
    return "Missing Fields";
  case types::ParseError::UnknownError:
    return "Unknown Error";
  case types::ParseError::InvalidLatitude:
    return "Invalid Latitude";
  case types::ParseError::InvalidLongitude:
    return "Invalid Longitude";
  case types::ParseError::InvalidSpeed:
    return "Invalid Speed";
  case types::ParseError::InvalidCourse:
    return "Invalid Course";
  case types::ParseError::InvalidUTCDate:
    return "Invalid UTC Date";
  case types::ParseError::InvalidUTCTime:
    return "Invalid UTC Time";
  case types::ParseError::InvalidMagneticVariation:
    return "Invalid Magnetic Variation";
  case types::ParseError::InvalidMode:
    return "Invalid Mode";
  }
  return "--";
}

} // namespace cnmea::p_tools
