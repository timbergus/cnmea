#pragma once

#include <expected>
#include <iomanip>
#include <iostream>
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
  if (status.empty()) {
    return types::Status::Void;
  }
  switch (status.front()) {
  case 'A':
    return types::Status::Active;
  case 'V':
    return types::Status::Void;
  }
  return types::Status::Void;
}

inline std::optional<types::Latitude>
parse_latitude(std::string_view value, std::string_view direction) {
  if (value.empty() || direction.empty()) {
    return std::nullopt;
  }
  auto latitude_value = parse_coordinate(value);
  auto latitude_direction = parse_latitude_direction(direction);
  if (latitude_value && latitude_direction) {
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
  if (longitude_value && longitude_direction) {
    return types::Longitude(longitude_value.value(),
                            longitude_direction.value());
  } else {
    return std::nullopt;
  }
}

inline std::optional<types::Speed>
parse_speed(std::string_view speed, types::Units units = types::Units::knots) {
  if (speed.empty()) {
    return std::nullopt;
  }
  auto speed_value = tools::parse_numeric_value(speed);
  if (speed_value) {
    switch (units) {
    case types::Units::ms:
      return types::Speed(speed_value.value() * types::KNTOMS, units);
    case types::Units::knots:
      return types::Speed(speed_value.value(), units);
    case types::Units::kmh:
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
  if (course_value) {
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
  if (magnetic_variation_value && magnetic_variation_direction) {
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

  switch (mode.front()) {
  case 'A':
    return types::Mode::Autonomous;
  case 'D':
    return types::Mode::Differential;
  case 'E':
    return types::Mode::Estimated;
  case 'N':
    return types::Mode::NotValid;
  default:
    return std::nullopt;
  }
}

inline types::Type parse_type(std::string_view type) {
  if (type.find("GGA") != std::string::npos) {
    return types::Type::GGA;
  } else if (type.find("RMC") != std::string::npos) {
    return types::Type::RMC;
  } else if (type.find("VTG") != std::string::npos) {
    return types::Type::VTG;
  }
  throw std::invalid_argument("Unknown type");
}

} // namespace cnmea::tools
