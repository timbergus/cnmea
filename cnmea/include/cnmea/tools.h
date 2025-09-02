#pragma once

#include <expected>
#include <iomanip>
#include <iostream>
#include <print>
#include <sstream>
#include <string_view>
#include <vector>

#include "types.h"

namespace cnmea::tools {

inline std::vector<std::string_view> split(const std::string_view sample,
                                           char separator) {
  size_t start = 0;
  size_t end = 0;

  std::vector<std::string_view> tokens;

  while ((end = sample.find(separator, start)) != std::string::npos) {
    tokens.push_back(sample.substr(start, end - start));
    start = end + 1;
  }

  tokens.push_back(sample.substr(start));

  return tokens;
}

inline bool is_valid_sample(const std::string_view sample) {
  std::vector<std::string_view> tokens{split(sample, '*')};

  if (tokens.size() < 2 || tokens.at(1).empty()) {
    return false;
  }

  std::string_view sentence = tokens.at(0);

  if (sentence.starts_with('$')) {
    sentence.remove_prefix(1);
  }

  unsigned char check = 0;

  for (char c : sentence) {
    check ^= static_cast<unsigned char>(c);
  }

  std::stringstream hex_check;

  hex_check << std::uppercase << std::hex << std::setfill('0') << std::setw(2)
            << static_cast<int>(check);

  return hex_check.str() == tokens.at(1);
}

inline std::vector<std::string_view> tokenize(const std::string_view sample) {
  std::vector<std::string_view> raw_data = split(sample, '*');
  return split(raw_data.at(0), ',');
}

inline std::expected<double, types::ParseError>
parse_numeric_value(const std::string_view token) {
  try {
    return std::stod(std::string{token});
  } catch (...) {
    return std::unexpected(types::ParseError::MissingFields);
  }
}

inline std::expected<double, types::ParseError>
parse_coordinate(const std::string_view token) {
  try {
    return std::stod(std::string{token}) / 100.0;
  } catch (...) {
    return std::unexpected(types::ParseError::MissingFields);
  }
}

inline auto parse_utc_time(std::string_view utc_time) {
  return types::UTCTime{
      std::string_view{utc_time.substr(0, 2)},
      std::string_view{utc_time.substr(2, 2)},
      std::string_view{utc_time.substr(4, 2)},
  };
}

inline std::expected<types::Direction, types::ParseError>
parse_latitude_direction(std::string_view token) {
  if (token == "N" || token == "S") {
    return token.front() == 'N' ? types::Direction::North
                                : types::Direction::South;
  }
  return std::unexpected(types::ParseError::InvalidDirection);
}

inline std::expected<types::Direction, types::ParseError>
parse_longitude_direction(std::string_view token) {
  if (!token.empty() && (token == "E" || token == "W")) {
    return token.front() == 'E' ? types::Direction::East
                                : types::Direction::West;
  }
  return std::unexpected(types::ParseError::InvalidDirection);
}

inline types::Status parse_status(std::string_view status) {
  using enum types::Status;
  switch (status.front()) {
  case 'A':
    return Valid;
  case 'V':
    return Invalid;
  default:
    throw std::invalid_argument("Unknown type");
  }
}

inline std::optional<types::Latitude>
parse_latitude(std::string_view value, std::string_view direction) {
  if (value.empty() || direction.empty()) {
    return std::nullopt;
  }
  auto latitude_value = parse_coordinate(value);
  auto latitude_direction = parse_latitude_direction(direction);
  if (latitude_value.has_value() && latitude_direction.has_value()) {
    return types::Latitude(latitude_value.value(), latitude_direction.value());
  } else {
    return std::nullopt;
  }
}

inline std::optional<types::Longitude>
parse_longitude(std::string_view value, std::string_view direction) {
  if (value.empty() || direction.empty()) {
    return std::nullopt;
  }
  auto longitude_value = parse_coordinate(value);
  auto longitude_direction = parse_longitude_direction(direction);
  if (longitude_value.has_value() && longitude_direction.has_value()) {
    return types::Longitude(longitude_value.value(),
                            longitude_direction.value());
  } else {
    return std::nullopt;
  }
}

inline std::optional<types::Speed>
parse_speed(std::string_view speed,
            types::SpeedUnits units = types::SpeedUnits::knots) {
  if (speed.empty()) {
    return std::nullopt;
  }
  auto speed_value = tools::parse_numeric_value(speed);
  if (speed_value.has_value()) {
    using enum types::SpeedUnits;
    switch (units) {
    case ms:
      return types::Speed(speed_value.value() * types::KNTOMS, units);
    case knots:
      return types::Speed(speed_value.value(), units);
    case kmh:
      return types::Speed(speed_value.value() * types::KNTOKMH, units);
    }
  } else {
    return std::nullopt;
  }
}

inline std::optional<types::Course> parse_course(std::string_view course) {
  if (course.empty()) {
    return std::nullopt;
  }
  auto course_value = tools::parse_numeric_value(course);
  if (course_value.has_value()) {
    return types::Course(course_value.value());
  } else {
    return std::nullopt;
  }
}

inline std::optional<types::UTCDate> parse_utc_date(std::string_view utc_date) {
  if (utc_date.empty() || utc_date.size() < 6) {
    return std::nullopt;
  }
  return types::UTCDate{
      std::string_view{utc_date.substr(0, 2)},
      std::string_view{utc_date.substr(2, 2)},
      std::string_view{utc_date.substr(4, 2)},
  };
};

inline std::optional<types::MagneticVariation>
parse_magnetic_variation(std::string_view value, std::string_view direction) {
  if (value.empty() || direction.empty()) {
    return std::nullopt;
  }
  auto magnetic_variation_value = parse_coordinate(value);
  auto magnetic_variation_direction = parse_longitude_direction(direction);
  if (magnetic_variation_value.has_value() &&
      magnetic_variation_direction.has_value()) {
    return types::MagneticVariation(magnetic_variation_value.value(),
                                    magnetic_variation_direction.value());
  } else {
    return std::nullopt;
  }
}

inline std::optional<types::Mode> parse_mode(std::string_view mode) {
  if (mode.empty()) {
    return std::nullopt;
  }

  using enum types::Mode;
  switch (mode.front()) {
  case 'A':
    return Autonomous;
  case 'D':
    return Differential;
  case 'E':
    return Estimated;
  case 'N':
    return NotValid;
  default:
    return std::nullopt;
  }
}

inline types::Type parse_type(std::string_view type) {
  using enum types::Type;
  if (type.contains("GGA")) {
    return GGA;
  } else if (type.contains("RMC")) {
    return RMC;
  } else if (type.contains("GLL")) {
    return GLL;
  } else if (type.contains("GSA")) {
    return GSA;
  } else if (type.contains("GSV")) {
    return GSV;
  } else if (type.contains("VTG")) {
    return VTG;
  } else if (type.contains("ZDA")) {
    return ZDA;
  }
  throw std::invalid_argument("Unknown type");
}

inline std::expected<types::DistanceUnits, types::ParseError>
parse_distance_units(std::string_view distance_units) {
  using enum types::DistanceUnits;
  if (distance_units == "M") {
    return m;
  } else if (distance_units == "KM") {
    return km;
  } else if (distance_units == "FT") {
    return ft;
  }
  return std::unexpected(types::ParseError::UnsupportedType);
}

inline types::FixQuality parse_fix_quality(std::string_view fix_quality) {
  using enum types::FixQuality;
  if (fix_quality == "0") {
    return Invalid;
  } else if (fix_quality == "1") {
    return GPS;
  } else if (fix_quality == "2") {
    return DGPS;
  } else if (fix_quality == "3") {
    return PPS;
  } else if (fix_quality == "4") {
    return RealTimeKinematic;
  } else if (fix_quality == "5") {
    return FloatRTK;
  } else if (fix_quality == "6") {
    return Estimated;
  } else if (fix_quality == "7") {
    return ManualInput;
  } else if (fix_quality == "8") {
    return Simulation;
  }

  throw std::invalid_argument("Unknown type");
}

inline std::optional<types::Altitude> parse_altitude(std::string_view altitude,
                                                     std::string_view units) {
  if (altitude.empty() || units.empty()) {
    return std::nullopt;
  }
  auto altitude_value = tools::parse_numeric_value(altitude);
  auto altitude_units = tools::parse_distance_units(units);
  if (altitude_value.has_value() && altitude_units.has_value()) {
    return types::Altitude(altitude_value.value(), altitude_units.value());
  } else {
    return std::nullopt;
  }
}

inline std::optional<types::GeoidSeparation>
parse_geoid_separation(std::string_view geoid_separation,
                       std::string_view units) {
  if (geoid_separation.empty() || units.empty()) {
    return std::nullopt;
  }
  auto geoid_separation_value = tools::parse_numeric_value(geoid_separation);
  auto geoid_separation_units = tools::parse_distance_units(units);
  if (geoid_separation_value.has_value() &&
      geoid_separation_units.has_value()) {
    return types::GeoidSeparation(geoid_separation_value.value(),
                                  geoid_separation_units.value());
  } else {
    return std::nullopt;
  }
}

inline std::optional<types::AgeOfDgps>
parse_age_of_dgps(std::string_view age_of_dgps) {
  if (age_of_dgps.empty()) {
    return std::nullopt;
  }
  auto age_of_dgps_value = tools::parse_numeric_value(age_of_dgps);
  if (age_of_dgps_value.has_value()) {
    return types::AgeOfDgps(age_of_dgps_value.value());
  } else {
    return std::nullopt;
  }
}

inline std::optional<types::DgpsStationId>
parse_dgps_station_id(std::string_view dgps_station_id) {
  if (dgps_station_id.empty()) {
    return std::nullopt;
  }
  auto dgps_station_id_value = tools::parse_numeric_value(dgps_station_id);
  if (dgps_station_id_value.has_value()) {
    return types::DgpsStationId(
        static_cast<int>(dgps_station_id_value.value()));
  } else {
    return std::nullopt;
  }
}

inline types::SelectionMode
parse_selection_mode(std::string_view selection_mode) {
  if (selection_mode == "M") {
    return types::SelectionMode::Manual;
  } else if (selection_mode == "A") {
    return types::SelectionMode::Automatic;
  }
  throw std::invalid_argument("Unknown type");
}

inline types::FixType parse_fix_type(std::string_view fix_type) {
  using enum types::FixType;
  if (fix_type == "1") {
    return None;
  } else if (fix_type == "2") {
    return TwoD;
  } else if (fix_type == "3") {
    return ThreeD;
  }
  throw std::invalid_argument("Unknown type");
}

inline std::optional<types::DOP>
parse_dop(std::string_view pdop, std::string_view hdop, std::string_view vdop) {
  if (pdop.empty() || hdop.empty() || vdop.empty()) {
    return std::nullopt;
  }

  auto pdop_value = tools::parse_numeric_value(pdop);
  auto hdop_value = tools::parse_numeric_value(hdop);
  auto vdop_value = tools::parse_numeric_value(vdop);

  if (pdop_value.has_value() && hdop_value.has_value() &&
      vdop_value.has_value()) {
    return types::DOP{pdop_value.value(), hdop_value.value(),
                      vdop_value.value()};
  } else {
    return std::nullopt;
  }
}

inline std::optional<types::Satellite>
parse_satellite(std::string_view prn, std::string_view snr,
                std::string_view elevation, std::string_view azimuth) {
  if (prn.empty()) {
    return std::nullopt;
  }

  auto prn_value = tools::parse_numeric_value(prn);
  auto snr_value = tools::parse_numeric_value(snr).value_or(
      std::numeric_limits<double>::quiet_NaN());
  auto elevation_value = tools::parse_numeric_value(elevation).value_or(
      std::numeric_limits<double>::quiet_NaN());
  auto azimuth_value = tools::parse_numeric_value(azimuth).value_or(
      std::numeric_limits<double>::quiet_NaN());

  if (prn_value.has_value()) {
    return types::Satellite{static_cast<int>(prn_value.value()), snr_value,
                            elevation_value, azimuth_value};
  } else {
    return std::nullopt;
  }
}

} // namespace cnmea::tools
