#pragma once

#include <cstdlib>
#include <expected>
#include <string>
#include <variant>

#include "gga.h"
#include "gll.h"
#include "gsa.h"
#include "gsv.h"
#include "rmc.h"
#include "types.h"
#include "vtg.h"
#include "zda.h"

namespace cnmea {

using GGA = gga::GGA;
using GLL = gll::GLL;
using GSA = gsa::GSA;
using GSV = gsv::GSV;
using RMC = rmc::RMC;
using VTG = vtg::VTG;
using ZDA = zda::ZDA;

using Sample = std::variant<GGA, GLL, GSA, GSV, RMC, VTG, ZDA>;

inline std::expected<Sample, types::ParseError> parse(std::string_view sample) {
  if (sample.contains("GGA")) {
    return gga::parse(sample);
  } else if (sample.contains("GLL")) {
    return gll::parse(sample);
  } else if (sample.contains("GSA")) {
    return gsa::parse(sample);
  } else if (sample.contains("GSV")) {
    return gsv::parse(sample);
  } else if (sample.contains("RMC")) {
    return rmc::parse(sample);
  } else if (sample.contains("VTG")) {
    return vtg::parse(sample);
  } else if (sample.contains("ZDA")) {
    return zda::parse(sample);
  }
  return std::unexpected(types::ParseError::UnsupportedType);
}

inline void print(const Sample &sample) {
  std::visit(
      [](auto &&data) {
        using T = std::decay_t<decltype(data)>;
        if constexpr (std::is_same_v<T, gga::GGA>) {
          gga::print(data);
        } else if constexpr (std::is_same_v<T, gll::GLL>) {
          gll::print(data);
        } else if constexpr (std::is_same_v<T, gsa::GSA>) {
          gsa::print(data);
        } else if constexpr (std::is_same_v<T, gsv::GSV>) {
          gsv::print(data);
        } else if constexpr (std::is_same_v<T, rmc::RMC>) {
          rmc::print(data);
        } else if constexpr (std::is_same_v<T, vtg::VTG>) {
          vtg::print(data);
        } else if constexpr (std::is_same_v<T, zda::ZDA>) {
          zda::print(data);
        } else {
          std::println("Print function not implemented for this type");
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
