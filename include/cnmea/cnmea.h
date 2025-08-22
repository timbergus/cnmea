#pragma once

#include <cstdlib>
#include <expected>
#include <string>
#include <variant>

#include "rmc.h"
#include "types.h"

namespace cnmea {

using RMC = rmc::RMC;

using Sample = std::variant<RMC>;

inline std::expected<Sample, types::ParseError> parse(std::string_view sample) {
  if (sample.find("RMC") != std::string::npos) {
    return rmc::parse(sample);
  }
  return std::unexpected(types::ParseError::UnsupportedType);
}

inline void print(const Sample &sample) {
  std::visit(
      [](auto &&data) {
        using T = std::decay_t<decltype(data)>;
        if constexpr (std::is_same_v<T, rmc::RMC>) {
          rmc::print(data);
        }
      },
      sample);
}

inline std::string to_string(const types::Element &element) {
  return std::visit(
      [](auto &&data) {
        using T = std::decay_t<decltype(data)>;
        if constexpr (std::is_same_v<T, types::ParseError>) {
          return p_tools::to_string(data);
        } else {
          return std::string("To string function not implemented for Element");
        }
      },
      element);
}

} // namespace cnmea
