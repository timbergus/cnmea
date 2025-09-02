#pragma once

#include <format>
#include <print>
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
    using enum types::Mode;
    switch (mode.value()) {
    case Autonomous:
      return "Autonomous";
    case Differential:
      return "Differential";
    case Estimated:
      return "Estimated";
    case ManualInput:
      return "Manual Input";
    case NotValid:
      return "Not Valid";
    case Simulation:
      return "Simulation";
    case Precise:
      return "Precise";
    case RTKFixed:
      return "RTK Fixed";
    case RTKFloat:
      return "RTK Float";
    case Uncalibrated:
      return "Uncalibrated";
    }
  }
  return "--";
}

inline std::string to_string(const types::SpeedUnits &speed_units) {
  using enum types::SpeedUnits;
  switch (speed_units) {
  case ms:
    return "m/s";
  case kmh:
    return "km/h";
  case knots:
    return "knots";
  }
  return "--";
}

inline std::string to_string(const types::DistanceUnits &distance_units) {
  using enum types::DistanceUnits;
  switch (distance_units) {
  case m:
    return "m";
  case km:
    return "km";
  case ft:
    return "ft";
  }
  return "--";
}

inline std::string to_string(const types::Direction &direction) {
  using enum types::Direction;
  switch (direction) {
  case North:
    return "North";
  case South:
    return "South";
  case East:
    return "East";
  case West:
    return "West";
  }
  return "--";
}

inline std::string to_string(const std::optional<types::Latitude> &latitude) {
  if (latitude.has_value()) {
    return std::format("{} {}", latitude->get_degrees(),
                       to_string(latitude->get_direction()));
  }
  return "--";
}

inline std::string to_string(const std::optional<types::Longitude> &longitude) {
  if (longitude.has_value()) {
    return std::format("{} {}", longitude->get_degrees(),
                       to_string(longitude->get_direction()));
  }
  return "--";
}

inline std::string to_string(const std::optional<types::Course> &course) {
  if (course.has_value()) {
    return std::format("{}", course->value_degrees());
  }
  return "--";
}

inline std::string to_string(const std::optional<types::UTCDate> &utc_date) {
  if (utc_date.has_value()) {
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
  using enum types::Type;
  switch (type) {
  case GGA:
    return "GGA";
  case GLL:
    return "GLL";
  case GSA:
    return "GSA";
  case GSV:
    return "GSV";
  case RMC:
    return "RMC";
  case VTG:
    return "VTG";
  case ZDA:
    return "ZDA";
  }

  return "--";
}

inline std::string to_string(const types::ParseError &error) {
  using enum types::ParseError;
  switch (error) {
  case InvalidFormat:
    return "Invalid Format";
  case UnsupportedType:
    return "Unsupported Type";
  case InvalidDirection:
    return "Invalid Direction";
  case MissingFields:
    return "Missing Fields";
  case UnknownError:
    return "Unknown Error";
  case InvalidLatitude:
    return "Invalid Latitude";
  case InvalidLongitude:
    return "Invalid Longitude";
  case InvalidSpeed:
    return "Invalid Speed";
  case InvalidCourse:
    return "Invalid Course";
  case InvalidUTCDate:
    return "Invalid UTC Date";
  case InvalidUTCTime:
    return "Invalid UTC Time";
  case InvalidMagneticVariation:
    return "Invalid Magnetic Variation";
  case InvalidMode:
    return "Invalid Mode";
  }
  return "--";
}

inline std::string
to_string(const std::optional<types::FixQuality> &fix_quality) {
  if (fix_quality) {
    using enum types::FixQuality;
    switch (fix_quality.value()) {
    case Invalid:
      return "Invalid";
    case GPS:
      return "GPS";
    case DGPS:
      return "DGPS";
    case PPS:
      return "PPS";
    case RealTimeKinematic:
      return "Real Time Kinematic";
    case FloatRTK:
      return "Float RTK";
    case Estimated:
      return "Estimated";
    case ManualInput:
      return "Manual Input";
    case Simulation:
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

inline std::string
to_string(const std::optional<types::SelectionMode> &selection_mode) {
  if (selection_mode) {
    switch (selection_mode.value()) {
    case types::SelectionMode::Manual:
      return "Manual";
    case types::SelectionMode::Automatic:
      return "Automatic";
    }
  }
  return "--";
}

inline std::string to_string(const std::optional<types::FixType> &fix_type) {
  if (fix_type) {
    using enum types::FixType;
    switch (fix_type.value()) {
    case None:
      return "None";
    case TwoD:
      return "2D";
    case ThreeD:
      return "3D";
    }
  }
  return "--";
}

inline std::string to_string(const std::optional<types::Satellite> &satellite) {
  if (satellite) {
    return std::format("PRN: {}, SNR: {}, Elevation: {}, Azimuth: {}",
                       satellite->prn, satellite->snr, satellite->elevation,
                       satellite->azimuth);
  }
  return "--";
}

inline std::string to_string(const std::optional<types::DOP> &dop) {
  if (dop) {
    return std::format("PDOP: {}, HDOP: {}, VDOP: {}", dop->pdop, dop->hdop,
                       dop->vdop);
  }
  return "--";
}

} // namespace cnmea::p_tools
