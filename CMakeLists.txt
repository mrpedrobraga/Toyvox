
# Set required version and policy version
cmake_minimum_required( VERSION 3.9.2 FATAL_ERROR )
cmake_policy( VERSION 3.9.2...3.13 )

# Prohibit screwing up the source tree
set( CMAKE_DISABLE_IN_SOURCE_BUILD ON )
set( CMAKE_DISABLE_SOURCE_CHANGES ON )

# Use C++11 everywhere
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

# Name the project and set a prefix for naming internal targets and variables
project( toyvox VERSION 0.01.00.01 LANGUAGES CXX C )
set( TARGET_PREFIX ${PROJECT_NAME}_ )

# Define some helper functions that will be visible further down the source tree
include( extern/cmake/helpers.cmake )

# Build and assemble third-party dependencies
add_subdirectory( extern )

# Build this project's targets
add_subdirectory( ToyvoxEngine )
