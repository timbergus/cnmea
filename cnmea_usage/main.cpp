#include <cnmea/cnmea.h>
#include <cstdlib>
#include <print>

int main() {
  std::string gga_sample =
      "$GNGGA,062735.00,3150.788156,N,11711.922383,E,1,12,2.0,90.0,M,,M,,*55";

  auto gga_result = cnmea::parse(gga_sample);

  if (gga_result) {
    cnmea::print(gga_result.value());
  } else {
    std::println("Error: {}", cnmea::to_string(gga_result.error()));
  }

  std::println("--------------------------------------------------");

  std::string rmc_sample =
      "$GNRMC,211041.00,A,4024.98796,N,00340.22512,W,0.027,,010218,,,D*7B";

  auto rmc_result = cnmea::parse(rmc_sample);

  if (rmc_result) {
    cnmea::print(rmc_result.value());
  } else {
    std::println("Error: {}", cnmea::to_string(rmc_result.error()));
  }

  std::println("--------------------------------------------------");

  std::string gll_sample =
      "$GNGLL,3150.788156,N,11711.922383,E,062735.00,A,A*76";

  auto gll_result = cnmea::gll::parse(gll_sample);

  if (gll_result) {
    cnmea::gll::print(gll_result.value());
  } else {
    std::println("Error: {}", cnmea::to_string(gll_result.error()));
  }

  return EXIT_SUCCESS;
}
