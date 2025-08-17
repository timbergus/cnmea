#pragma once

#include <string>

namespace cnmea {

inline bool is_valid_sample(const std::string &sample) {
  if (sample.empty()) {
    return false; // Invalid if empty
  }
  return true;
}

} // namespace cnmea
