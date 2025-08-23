#pragma once

#include <expected>
#include <optional>
#include <print>
#include <string_view>
#include <vector>

#include "p_tools.h"
#include "tools.h"
#include "types.h"

namespace cnmea::gsa {

/// @brief GNSS DOP and Active Satellites (GSA) sentence.
///
/// Example:
/// @code
/// using namespace cnmea::types;
/// using namespace cnmea::sentences;
///
/// GSA gsa_example{
///     "GSA",
///     SelectionMode::Automatic,
///     FixType::ThreeD,
///     {Satellite{12}, Satellite{24}, Satellite{32}}, // satellites in use
///     1.8, 1.0, 1.5 // PDOP, HDOP, VDOP
/// };
///
/// for (const auto& sat : gsa_example.satellites) {
///     std::printf("Using satellite PRN %d\n", sat.prn);
/// }
/// @endcode
struct GSA {
  types::Type type;                         ///< Sentence type ("GSA")
  types::SelectionMode selection_mode;      ///< Manual or Automatic
  types::FixType fix_type;                  ///< Fix type (None, 2D, 3D)
  std::vector<types::Satellite> satellites; ///< Satellites used in solution
  std::optional<types::DOP> dop; ///< Dilution of Precision (DOP) values
};

inline std::expected<GSA, types::ParseError> parse(std::string_view sample) {
  if (!tools::is_valid_sample(sample)) {
    return std::unexpected(types::ParseError::InvalidFormat);
  }

  auto tokens = tools::tokenize(sample);

  if (tokens.empty()) {
    return std::unexpected(types::ParseError::UnknownError);
  }

  GSA gsa;

  // Example: $GNGSA,A,3,02,04,05,12,13,,,,,,,,1.8,1.0,1.5*33
  // tokens[0] = "$GNGSA"
  // tokens[1] = Mode (A/M)
  // tokens[2] = FixType (1=No fix, 2=2D, 3=3D)
  // tokens[3..14] = up to 12 PRNs
  // tokens[15] = PDOP
  // tokens[16] = HDOP
  // tokens[17] = VDOP

  gsa.type = tools::parse_type(tokens.at(0));
  gsa.selection_mode = tools::parse_selection_mode(tokens.at(1));
  gsa.fix_type = tools::parse_fix_type(tokens.at(2));

  // Satellites (just PRNs in GSA, no SNR/elev/azimuth)
  for (size_t i = 3; i <= 14 && i < tokens.size(); i++) {
    auto sat = tools::parse_satellite(tokens.at(i), "", "",
                                      ""); // Only PRN is available in GSA.

    if (sat) {
      gsa.satellites.push_back(sat.value());
    }
  }

  // DOP values (if present)
  if (tokens.size() >= 18) {
    gsa.dop = tools::parse_dop(std::string_view{tokens.at(15)},
                               std::string_view{tokens.at(16)},
                               std::string_view{tokens.at(17)});
  }

  return gsa;
}

inline void print(const GSA &data) {
  std::println("Type: {}", p_tools::to_string(data.type));
  std::println("Selection Mode: {}", p_tools::to_string(data.selection_mode));
  std::println("Fix Type: {}", p_tools::to_string(data.fix_type));
  std::println("Satellites:");
  for (const auto &sat : data.satellites) {
    std::println("  {}", p_tools::to_string(sat));
  }
  std::println("DOP: {}", p_tools::to_string(data.dop));
}

} // namespace cnmea::gsa
