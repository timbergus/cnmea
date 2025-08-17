#include <cstdlib>
#include <iostream>

#include <cnmea/cnmea.h>

int main() {
  std::cout << "Testing cnmea library..." << std::endl;

  // Call something from your library
  const bool is_valid = cnmea::is_valid_sample("my_sample_data");
  std::cout << "Sample is " << (is_valid ? "valid" : "invalid") << std::endl;

  return EXIT_SUCCESS;
}
