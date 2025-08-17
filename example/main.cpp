#include <cstdlib>
#include <iostream>

#include "cnmea/tools.h"

int main() {
  std::cout << "Testing cnmea library..." << std::endl;

  // Call something from your library
  const bool is_valid = cnmea::is_valid_sample("*3");
  std::cout << "Sample '*3' is " << (is_valid ? "valid" : "invalid")
            << std::endl;

  return EXIT_SUCCESS;
}
