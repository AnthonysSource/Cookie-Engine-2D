cmake_minimum_required(VERSION 3.22.1)
project(Cookie)

# C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build configurations
set(BUILD_TYPES "Debug" "Release" "Dist")

# Define build directories for multi-config generators (like visual studio)
set(BUILD_DIR_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug/)
set(BUILD_DIR_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release/)
set(BUILD_DIR_DIST ${CMAKE_CURRENT_SOURCE_DIR}/bin/Dist/)

# Check if the generator is multi-config
get_property(multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

if(multi_config)
  set(CMAKE_CONFIGURATION_TYPES "${BUILD_TYPES}" CACHE STRING "list of supported configuration types" FORCE)
else()
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build Type of the project.")
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${BUILD_TYPES}")
  if(NOT CMAKE_BUILD_TYPE IN_LIST BUILD_TYPES)
    message(FATAL_ERROR
      "Invalid build type '${CMAKE_BUILD_TYPE}'.
      CMAKE_BUILD_TYPE must be any one of the possible values:
      ${BUILD_TYPES}"
    )
  endif()
endif()

add_subdirectory(source/Cookie_Engine/code)