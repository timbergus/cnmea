cmake_minimum_required(VERSION 4.1.0)
project(
  cnmea
  VERSION 0.0.7
  DESCRIPTION "C NMEA 0183 Parser"
  LANGUAGES C CXX
)

set(PROJECT_AUTHOR "Gustavo Muñoz")

add_compile_options(-Wall -Wextra -Werror -Wpedantic)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
  CMAKE_CXX_STANDARD 20
  CMAKE_CXX_STANDARD_REQUIRED ON
  CMAKE_CXX_EXTENSIONS OFF
  CMAKE_EXPORT_COMPILE_COMMANDS ON
)

include(GNUInstallDirs)

target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# This is the testing target for internal development.
add_executable(${PROJECT_NAME}_test test/main.cpp)
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME})

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add_compile_options(-Wall -Wextra -Werror -Wpedantic)

# # >>> nlohmann_json download
# find_package(nlohmann_json QUIET)

# if(NOT nlohmann_json_FOUND)
#   include(FetchContent)

#   FetchContent_Declare(
#     nlohmann_json
#     GIT_REPOSITORY https://github.com/nlohmann/json.git
#     GIT_TAG v3.12.0
#   )
#   FetchContent_MakeAvailable(nlohmann_json)
# endif()
# # <<< nlohmann_json download

# add_library(${PROJECT_NAME} INTERFACE)
# target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_26)

# target_include_directories(
#   ${PROJECT_NAME}
#   INTERFACE
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#   $<INSTALL_INTERFACE:include>
# )

# # target_link_libraries(${PROJECT_NAME} INTERFACE nlohmann_json::nlohmann_json)

# add_executable(${PROJECT_NAME}_test test/main.cpp)
# target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME})

# # Installation rules
# include(GNUInstallDirs)

# install(TARGETS ${PROJECT_NAME}
#   EXPORT ${PROJECT_NAME}Targets
#   INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )

# install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# # Export targets for find_package()
# install(EXPORT ${PROJECT_NAME}Targets
#   FILE ${PROJECT_NAME}Targets.cmake
#   NAMESPACE ${PROJECT_NAME}::
#   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
# )

# # Optional: version file
# include(CMakePackageConfigHelpers)

# write_basic_package_version_file(
#   ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
#   VERSION ${PROJECT_VERSION}
#   COMPATIBILITY SameMajorVersion
# )

# configure_package_config_file(
#   ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
#   ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
#   INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
# )

# install(FILES
#   ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
#   ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
#   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
# )

# >>> Doxygen setup
find_package(Doxygen REQUIRED)

if (DOXYGEN_FOUND)
  set(DOXYGEN_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/docs)
  set(DOXYGEN_INPUT_DIR ${CMAKE_SOURCE_DIR}/src)

  # file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

  set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/Doxyfile)
  set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/Doxyfile)

  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

  add_custom_target(docs
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
  )

  install(DIRECTORY ${DOXYGEN_OUTPUT_DIR}/html/
    DESTINATION docs
    COMPONENT docs
  )
endif()
# <<< Doxygen setup

# >>> Testing
include(CTest)
enable_testing()
# <<< Testing

# >>> CPack metadata
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_DESCRIPTION ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR ${PROJECT_AUTHOR})
set(CPACK_GENERATOR "ZIP;TGZ")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
include(CPack)
# <<< CPack metadata
