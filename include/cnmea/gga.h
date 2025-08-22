#pragma once

#include <expected>
#include <optional>
#include <print>

#include "p_tools.h"
#include "tools.h"
#include "types.h"

namespace cnmea::gga {

struct GGA {
  types::Type type;
  types::UTCTime utc_time;
  std::optional<types::Latitude> latitude;
  std::optional<types::Longitude> longitude;
  types::FixQuality fix_quality;
  int num_satellites;
  double hdop;
  std::optional<types::Altitude> altitude;
  std::optional<types::GeoidSeparation> geoid_separation;
  std::optional<types::AgeOfDgps> age_of_dgps;
  std::optional<types::DgpsStationId> dgps_station_id;
};

inline std::expected<GGA, types::ParseError> parse(std::string_view sample) {
  if (!tools::is_valid_sample(sample)) {
    return std::unexpected(types::ParseError::InvalidFormat);
  }

  auto tokens = tools::tokenize(sample);

  if (tokens.empty()) {
    return std::unexpected(types::ParseError::UnknownError);
  }

  return GGA{
      tools::parse_type(tokens.at(0)),
      tools::parse_utc_time(tokens.at(1)),
      tools::parse_latitude(tokens.at(2), tokens.at(3)),
      tools::parse_longitude(tokens.at(4), tokens.at(5)),
      tools::parse_fix_quality(tokens.at(6)),
      static_cast<int>(tools::parse_numeric_value(tokens.at(7)).value_or(0)),
      tools::parse_numeric_value(tokens.at(8)).value_or(0.0),
      tools::parse_altitude(tokens.at(9), tokens.at(10)),
      tools::parse_geoid_separation(tokens.at(11), tokens.at(12)),
      tools::parse_age_of_dgps(tokens.at(13)),
      tools::parse_dgps_station_id(tokens.at(14)),
  };
}

inline void print(const GGA &data) {
  std::println("Type: {}", p_tools::to_string(data.type));
  std::println("UTC Time: {}", p_tools::to_string(data.utc_time));
  std::println("Latitude: {}", p_tools::to_string(data.latitude));
  std::println("Longitude: {}", p_tools::to_string(data.longitude));
  std::println("Fix Quality: {}", p_tools::to_string(data.fix_quality));
  std::println("Number of Satellites: {}", data.num_satellites);
  std::println("HDOP: {}", data.hdop);
  std::println("Altitude: {}", p_tools::to_string(data.altitude));
  std::println("Geoid Separation: {}",
               p_tools::to_string(data.geoid_separation));
  std::println("Age of DGPS: {}", p_tools::to_string(data.age_of_dgps));
  std::println("DGPS Station ID: {}", p_tools::to_string(data.dgps_station_id));
}

} // namespace cnmea::gga
