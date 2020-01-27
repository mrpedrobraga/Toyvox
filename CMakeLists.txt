# Set required version and policy version
cmake_minimum_required( VERSION 3.9.2 FATAL_ERROR )
cmake_policy( VERSION 3.9.2...3.16 )

message( "root:" )

# Prohibit screwing up the source tree
set( CMAKE_DISABLE_IN_SOURCE_BUILD ON )
set( CMAKE_DISABLE_SOURCE_CHANGES ON )

# Use C++14 everywhere
set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

# Name the project and set a prefix for naming internal targets and variables
project( Toyvox VERSION 0.01.00.01 LANGUAGES CXX C )
set( TARGET_PREFIX ${PROJECT_NAME} )

# Handy Helpers
include( helpers.cmake )

# Statically link standard libraries where possible (temporary aid for ease of testing)
enable_cxx_compiler_flag_if_supported( -static )
enable_cxx_compiler_flag_if_supported( -static-libgcc )
enable_cxx_compiler_flag_if_supported( -static-libstdc++ )
set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

# Build and assemble third-party dependencies
add_subdirectory( "./extern" )

# Build this project's targets
add_subdirectory( "./tvxsrc" )