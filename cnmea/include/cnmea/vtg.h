#pragma once

#include <expected>
#include <optional>

#include "p_tools.h"
#include "tools.h"
#include "types.h"

namespace cnmea::vtg {

struct VTG {
  types::Type type;
  std::optional<types::Course> course_true;
  std::optional<types::Course> course_magnetic;
  std::optional<types::Speed> speed_knots;
  std::optional<types::Speed> speed_kmh;
  std::optional<types::Mode> mode;
};

inline std::expected<VTG, types::ParseError> parse(std::string_view sample) {
  if (!tools::is_valid_sample(sample)) {
    return std::unexpected(types::ParseError::InvalidFormat);
  }

  auto tokens = tools::tokenize(sample);

  if (tokens.empty()) {
    return std::unexpected(types::ParseError::UnknownError);
  }

  return VTG{
      tools::parse_type(tokens.at(0)),
      tools::parse_course(tokens.at(1)),
      tools::parse_course(tokens.at(3)),
      tools::parse_speed(tokens.at(5), types::SpeedUnits::knots),
      tools::parse_speed(tokens.at(7), types::SpeedUnits::kmh),
      tools::parse_mode(tokens.at(9)),
  };
}

inline void print(const VTG &data) {
  std::println("Type: {}", p_tools::to_string(data.type));
  std::println("Course True: {}", p_tools::to_string(data.course_true));
  std::println("Course Magnetic: {}", p_tools::to_string(data.course_magnetic));
  std::println("Speed: {}", p_tools::to_string(data.speed_knots));
  std::println("Speed: {}", p_tools::to_string(data.speed_kmh));
  std::println("Mode: {}", p_tools::to_string(data.mode));
}

} // namespace cnmea::vtg
