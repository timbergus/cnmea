cmake_minimum_required(VERSION 4.1.0)
project(
  cnmea
  VERSION 0.1.0
  DESCRIPTION "C NMEA 0183 Parser"
  LANGUAGES C CXX
)

set(PROJECT_AUTHOR "Gustavo Muñoz")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Werror -Wpedantic)

# >>> nlohmann_json download
find_package(nlohmann_json QUIET)

if(NOT nlohmann_json_FOUND)
  include(FetchContent)

  FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
  )
  FetchContent_MakeAvailable(nlohmann_json)
endif()
# <<< nlohmann_json download

add_library(${PROJECT_NAME} INTERFACE)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_26)

target_include_directories(
  ${PROJECT_NAME}
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# target_link_libraries(${PROJECT_NAME} INTERFACE nlohmann_json::nlohmann_json)

# Installation rules
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export targets for find_package()
install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Optional: version file
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# >>> Testing
include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
# <<< Testing
