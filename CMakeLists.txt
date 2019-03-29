
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

# Define functions to enable/disable compiler flags depending on compiler support
include( CheckCXXCompilerFlag )
# Enable a flag if the compiler you're using supports it: enable_cxx_compiler_flag_if_supported( "flag" )
function( enable_cxx_compiler_flag_if_supported flag )
  string( FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set )
  if( flag_already_set EQUAL -1 )
    check_cxx_compiler_flag( "${flag}" flag_supported )
    if( flag_supported )
      set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" )
    endif()
    unset( flag_supported CACHE )
  endif()
endfunction()
# [Experimental] Disable a flag if it's currently enabled: disable_cxx_compiler_flag_if_enabled( "flag" )
function( disable_cxx_compiler_flag_if_enabled flag )
  string( REPLACE "${flag}"
    "" modified_flags
    "${CMAKE_CXX_FLAGS}"
    )
  set( CMAKE_CXX_FLAGS "${modified_flags}" )
endfunction()

# Build and assemble third-party dependencies
add_subdirectory( "./extern" )

# Build this project's targets
add_subdirectory( "./ToyvoxEngine" )
