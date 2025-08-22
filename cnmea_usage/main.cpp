#include <cnmea/cnmea.h>
#include <cstdlib>
#include <print>

int main() {
  std::string sample =
      "$GNRMC,211041.00,A,4024.98796,N,00340.22512,W,0.027,,010218,,,D*7B";

  auto result = cnmea::parse(sample);

  if (result) {
    cnmea::print(result.value());
  } else {
    std::println("Error: {}", cnmea::to_string(result.error()));
  }

  return EXIT_SUCCESS;
}
