cmake_minimum_required(VERSION 4.1.1)
project(
  cnmea
  VERSION
    0.0.11
  DESCRIPTION
    "C NMEA 0183 Parser"
  LANGUAGES
    C
    CXX
)

set(
  PROJECT_AUTHOR
    "Gustavo Muñoz"
)

set(
  MY_WARNINGS
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)

# >>> Library definition
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME}
  ALIAS
    ${PROJECT_NAME}
)

# Tell consumers where to find headers
target_include_directories(${PROJECT_NAME}
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_options(${PROJECT_NAME}
  INTERFACE
    ${MY_WARNINGS}
)
# <<< Library definition

# >>> Install configuration
include(GNUInstallDirs)

install(
  TARGETS
    ${PROJECT_NAME}
  EXPORT
    ${PROJECT_NAME}Targets
)

install(
  EXPORT
    ${PROJECT_NAME}Targets
  FILE
    ${PROJECT_NAME}Config.cmake
  NAMESPACE
    ${PROJECT_NAME}::
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
  DIRECTORY
    ${PROJECT_NAME}/include/
  DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}
)
# <<< Install rules

# >>> Example program
add_executable(${PROJECT_NAME}_usage ${PROJECT_NAME}_usage/main.cpp)

target_compile_options(${PROJECT_NAME}_usage
  PRIVATE
    ${MY_WARNINGS}
)

target_link_libraries(${PROJECT_NAME}_usage
  PRIVATE
    ${PROJECT_NAME}::${PROJECT_NAME}
)
# <<< Example program

# >>> Doxygen setup
find_package(Doxygen REQUIRED)

if (DOXYGEN_FOUND)
  set(DOXYGEN_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/docs)
  set(DOXYGEN_INPUT_DIR ${CMAKE_SOURCE_DIR}/${PROJECT_NAME})

  set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/Doxyfile)
  set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/Doxyfile)

  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

  add_custom_target(
    ${PROJECT_NAME}_docs
    COMMAND
      ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY
      ${CMAKE_SOURCE_DIR}
    COMMENT
      "Generating API documentation with Doxygen"
    VERBATIM
  )
endif()
# <<< Doxygen setup

# >>> Testing
include(CTest)
enable_testing()
# <<< Testing
