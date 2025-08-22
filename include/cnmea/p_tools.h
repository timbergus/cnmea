#pragma once

#include <format>
#include <string>

#include "types.h"

namespace cnmea::p_tools {

inline std::string to_string(const types::Status &status) {
  switch (status) {
  case types::Status::Valid:
    return "Valid";
  case types::Status::Invalid:
    return "Invalid";
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
    case types::Mode::ManualInput:
      return "Manual Input";
    case types::Mode::NotValid:
      return "Not Valid";
    case types::Mode::Simulation:
      return "Simulation";
    case types::Mode::Precise:
      return "Precise";
    case types::Mode::RTKFixed:
      return "RTK Fixed";
    case types::Mode::RTKFloat:
      return "RTK Float";
    case types::Mode::Uncalibrated:
      return "Uncalibrated";
    }
  }
  return "--";
}

inline std::string to_string(const types::SpeedUnits &speed_units) {
  switch (speed_units) {
  case types::SpeedUnits::ms:
    return "m/s";
  case types::SpeedUnits::kmh:
    return "km/h";
  case types::SpeedUnits::knots:
    return "knots";
  }
  return "--";
}

inline std::string to_string(const types::DistanceUnits &distance_units) {
  switch (distance_units) {
  case types::DistanceUnits::m:
    return "m";
  case types::DistanceUnits::km:
    return "km";
  case types::DistanceUnits::ft:
    return "ft";
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
  return "--/--/--";
}

inline std::string to_string(const types::UTCTime &utc_time) {
  if (!(utc_time.hours.empty() || utc_time.minutes.empty() ||
        utc_time.seconds.empty())) {

    return std::format("{}:{}:{}", utc_time.hours, utc_time.minutes,
                       utc_time.seconds);
  }
  return "--:--:--";
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
  case types::Type::GLL:
    return "GLL";
  case types::Type::VTG:
    return "VTG";
  case types::Type::ZDA:
    return "ZDA";
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

inline std::string
to_string(const std::optional<types::FixQuality> &fix_quality) {
  if (fix_quality) {
    switch (fix_quality.value()) {
    case types::FixQuality::Invalid:
      return "Invalid";
    case types::FixQuality::GPS:
      return "GPS";
    case types::FixQuality::DGPS:
      return "DGPS";
    case types::FixQuality::PPS:
      return "PPS";
    case types::FixQuality::RealTimeKinematic:
      return "Real Time Kinematic";
    case types::FixQuality::FloatRTK:
      return "Float RTK";
    case types::FixQuality::Estimated:
      return "Estimated";
    case types::FixQuality::ManualInput:
      return "Manual Input";
    case types::FixQuality::Simulation:
      return "Simulation";
    }
  }
  return "--";
}

inline std::string to_string(const std::optional<types::Altitude> &altitude) {
  if (altitude) {
    return std::format("{} {}", altitude->value_meters(),
                       to_string(altitude->get_units()));
  }
  return "--";
}

inline std::string
to_string(const std::optional<types::GeoidSeparation> &geoid_separation) {
  if (geoid_separation) {
    return std::format("{} {}", geoid_separation->value_meters(),
                       to_string(geoid_separation->get_units()));
  }
  return "--";
}

inline std::string
to_string(const std::optional<types::AgeOfDgps> &age_of_dgps) {
  if (age_of_dgps) {
    return std::format("{}", age_of_dgps->value_seconds());
  }
  return "--";
}

inline std::string
to_string(const std::optional<types::DgpsStationId> &dgps_station_id) {
  if (dgps_station_id) {
    return std::format("{}", dgps_station_id->value());
  }
  return "--";
}

} // namespace cnmea::p_tools
