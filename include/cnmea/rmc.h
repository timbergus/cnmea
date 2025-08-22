#pragma once

#include <expected>
#include <optional>
#include <print>
#include <string_view>

#include "p_tools.h"
#include "tools.h"
#include "types.h"

namespace cnmea::rmc {

struct RMC {
  types::Type type;
  types::UTCTime utc_time;
  types::Status status;
  std::optional<types::Latitude> latitude;
  std::optional<types::Longitude> longitude;
  std::optional<types::Speed> speed;
  std::optional<types::Course> course;
  std::optional<types::UTCDate> utc_date;
  std::optional<types::MagneticVariation> magnetic_variation;
  std::optional<types::Mode> mode;
};

inline std::expected<RMC, types::ParseError> parse(std::string_view sample) {
  if (!tools::is_valid_sample(sample)) {
    return std::unexpected(types::ParseError::InvalidFormat);
  }

  auto tokens = tools::tokenize(sample);

  if (tokens.empty()) {
    return std::unexpected(types::ParseError::UnknownError);
  }

  return RMC{
      tools::parse_type(tokens.at(0)),
      tools::parse_utc_time(tokens.at(1)),
      tools::parse_status(tokens.at(2)),
      tools::parse_latitude(tokens.at(3), tokens.at(4)),
      tools::parse_longitude(tokens.at(5), tokens.at(6)),
      tools::parse_speed(tokens.at(7)),
      tools::parse_course(tokens.at(8)),
      tools::parse_utc_date(tokens.at(9)),
      tools::parse_magnetic_variation(tokens.at(10), tokens.at(11)),
      tools::parse_mode(tokens.at(12)),
  };
}

inline void print(const RMC &data) {
  std::println("Type: {}", p_tools::to_string(data.type));
  std::println("Status: {}", p_tools::to_string(data.status));
  std::println("UTC Date: {}", p_tools::to_string(data.utc_date));
  std::println("UTC Time: {}", p_tools::to_string(data.utc_time));
  std::println("Latitude: {}", p_tools::to_string(data.latitude));
  std::println("Longitude: {}", p_tools::to_string(data.longitude));
  std::println("Speed: {}", p_tools::to_string(data.speed));
  std::println("Course: {}", p_tools::to_string(data.course));
  std::println("Magnetic Variation: {}",
               p_tools::to_string(data.magnetic_variation));
  std::println("Mode: {}", p_tools::to_string(data.mode));
}

} // namespace cnmea::rmc
