#pragma once

#include <expected>
#include <vector>

#include "p_tools.h"
#include "tools.h"
#include "types.h"

namespace cnmea::gsv {

struct GSV {
  types::Type type;       ///< Sentence type ("GSV")
  int total_messages;     ///< Total number of GSV sentences for this cycle
  int message_number;     ///< Sentence number within this cycle
  int satellites_in_view; ///< Total satellites in view
  std::vector<types::Satellite> satellites; ///< Up to 4 satellites per sentence
};

inline std::expected<GSV, types::ParseError> parse(std::string_view sample) {
  if (!tools::is_valid_sample(sample)) {
    return std::unexpected(types::ParseError::InvalidFormat);
  }

  auto tokens = tools::tokenize(sample);

  if (tokens.empty()) {
    return std::unexpected(types::ParseError::UnknownError);
  }

  GSV gsv;

  gsv.type = tools::parse_type(tokens[0]);
  gsv.total_messages = tools::parse_numeric_value(tokens[1]).value_or(0);
  gsv.message_number = tools::parse_numeric_value(tokens[2]).value_or(0);
  gsv.satellites_in_view = tools::parse_numeric_value(tokens[3]).value_or(0);

  // Parse satellite information (up to 4 satellites)
  for (size_t i = 4; i < tokens.size(); i += 4) {
    if (i + 3 < tokens.size()) {
      auto sat = tools::parse_satellite(tokens[i], tokens[i + 1], tokens[i + 2],
                                        tokens[i + 3]);
      if (sat) {
        gsv.satellites.push_back(sat.value());
      }
    }
  }

  return gsv;
}

inline void print(const GSV &data) {
  std::println("Type: {}", p_tools::to_string(data.type));
  std::println("Total Messages: {}", data.total_messages);
  std::println("Message Number: {}", data.message_number);
  std::println("Satellites in View: {}", data.satellites_in_view);
  std::println("Satellites:");
  for (const auto &sat : data.satellites) {
    std::println("  {}", p_tools::to_string(sat));
  }
}

} // namespace cnmea::gsv
