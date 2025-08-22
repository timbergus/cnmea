#pragma once

#include <expected>
#include <optional>
#include <print>

#include "p_tools.h"
#include "tools.h"
#include "types.h"

namespace cnmea::gll {

struct GLL {
  types::Type type;
  std::optional<types::Latitude> latitude;
  std::optional<types::Longitude> longitude;
  types::UTCTime utc_time;         // hhmmss.sss
  types::Status status;            // A=valid, V=invalid
  std::optional<types::Mode> mode; // A, D, E... (optional, only in NMEA 2.3+)
};

inline std::expected<GLL, types::ParseError> parse(std::string_view sample) {
  if (!tools::is_valid_sample(sample)) {
    return std::unexpected(types::ParseError::InvalidFormat);
  }

  auto tokens = tools::tokenize(sample);

  if (tokens.empty()) {
    return std::unexpected(types::ParseError::UnknownError);
  }

  return GLL{
      tools::parse_type(tokens.at(0)),
      tools::parse_latitude(tokens.at(1), tokens.at(2)),
      tools::parse_longitude(tokens.at(3), tokens.at(4)),
      tools::parse_utc_time(tokens.at(5)),
      tools::parse_status(tokens.at(6)),
      tools::parse_mode(tokens.at(7)),
  };
}

inline void print([[maybe_unused]] const GLL &data) {
  std::println("Type: {}", p_tools::to_string(data.type));
  std::println("Latitude: {}", p_tools::to_string(data.latitude));
  std::println("Longitude: {}", p_tools::to_string(data.longitude));
  std::println("UTC Time: {}", p_tools::to_string(data.utc_time));
  std::println("Status: {}", p_tools::to_string(data.status));
  std::println("Mode: {}", p_tools::to_string(data.mode));
}

} // namespace cnmea::gll
