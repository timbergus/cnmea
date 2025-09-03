cmake_minimum_required(VERSION 4.1.1)

project(
  cnmea
  VERSION 0.0.12
  DESCRIPTION "C NMEA 0183 Parser"
  LANGUAGES C CXX
)

# >>> Project metadata
set(PROJECT_AUTHOR "Gustavo Muñoz")

# Language standards (safe even without presets)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Common warnings
set(MY_WARNINGS -Wall -Wextra -Wpedantic -Werror)
# <<< Project metadata

# >>> Library definition (header-only, INTERFACE)
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Tell consumers where to find headers
target_include_directories(${PROJECT_NAME}
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_options(${PROJECT_NAME} INTERFACE ${MY_WARNINGS})
# <<< Library definition

# >>> Install configuration
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Export targets
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
)

# Config and version files for find_package()
install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Config.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Headers
install(
  DIRECTORY ${PROJECT_NAME}/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
# <<< Install rules

# >>> Example program
add_executable(${PROJECT_NAME}_usage ${PROJECT_NAME}_usage/main.cpp)

target_link_libraries(${PROJECT_NAME}_usage
  PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
)
# <<< Example program

# >>> Documentation (optional if Doxygen not installed)
find_package(Doxygen QUIET)

if (DOXYGEN_FOUND)
  set(DOXYGEN_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/docs)
  set(DOXYGEN_INPUT_DIR ${CMAKE_SOURCE_DIR}/${PROJECT_NAME})

  set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/Doxyfile)
  set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/Doxyfile)

  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

  add_custom_target(
    ${PROJECT_NAME}_docs
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
  )
else()
  message(STATUS "Doxygen not found, skipping documentation target")
endif()
# <<< Documentation (optional if Doxygen not installed)

# >>> Testing
include(CTest)
enable_testing()
# <<< Testing
