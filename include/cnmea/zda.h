#pragma once

#include <expected>
#include <optional>

#include "p_tools.h"
#include "tools.h"
#include "types.h"

namespace cnmea::zda {

struct ZDA {
  types::Type type;
  types::UTCTime utc_time;
  int day;
  int month;
  int year;
  std::optional<int> local_zone_hours;
  std::optional<int> local_zone_minutes;
};

inline std::expected<ZDA, types::ParseError> parse(std::string_view sample) {
  if (!tools::is_valid_sample(sample)) {
    return std::unexpected(types::ParseError::InvalidFormat);
  }

  auto tokens = tools::tokenize(sample);

  if (tokens.empty()) {
    return std::unexpected(types::ParseError::UnknownError);
  }

  return ZDA{
      tools::parse_type(tokens.at(0)),
      tools::parse_utc_time(tokens.at(1)),
      tokens.size() > 2 && !tokens.at(2).empty()
          ? std::stoi(std::string{tokens.at(2)})
          : 0,
      tokens.size() > 3 && !tokens.at(3).empty()
          ? std::stoi(std::string{tokens.at(3)})
          : 0,
      tokens.size() > 4 && !tokens.at(4).empty()
          ? std::stoi(std::string{tokens.at(4)})
          : 0,
      tokens.size() > 5 && !tokens.at(5).empty()
          ? std::stoi(std::string{tokens.at(5)})
          : 0,
      tokens.size() > 6 && !tokens.at(6).empty()
          ? std::stoi(std::string{tokens.at(6)})
          : 0,
  };
}

inline void print(const ZDA &data) {
  std::println("Type: {}", p_tools::to_string(data.type));
  std::println("UTC Time: {}", p_tools::to_string(data.utc_time));
  std::println("Day: {}", data.day);
  std::println("Month: {}", data.month);
  std::println("Year: {}", data.year);
  if (data.local_zone_hours) {
    std::println("Local Zone Hours: {}", *data.local_zone_hours);
  }
  if (data.local_zone_minutes) {
    std::println("Local Zone Minutes: {}", *data.local_zone_minutes);
  }
}

} // namespace cnmea::zda
