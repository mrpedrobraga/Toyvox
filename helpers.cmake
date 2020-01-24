

# Define functions to enable/disable compiler flags depending on compiler support
include( CheckCXXCompilerFlag )
# Enable a flag if the compiler you're using supports it: enable_cxx_compiler_flag_if_supported( "flag" )
function( enable_cxx_compiler_flag_if_supported flag )
  string( FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set )
  if ( flag_already_set EQUAL -1 )
    check_cxx_compiler_flag( "${flag}" flag_supported )
    if ( flag_supported )
      set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" )
      message( STATUS "COMPILER flag set: ${flag}" )
    endif ()
    unset( flag_supported CACHE )
  endif ()
endfunction()
# [Experimental] Disable a flag if it's currently enabled: disable_cxx_compiler_flag_if_enabled( "flag" )
function( disable_cxx_compiler_flag_if_enabled flag )
  string( REPLACE "${flag}"
    "" modified_flags
    "${CMAKE_CXX_FLAGS}"
    )
  set( CMAKE_CXX_FLAGS "${modified_flags}" )
endfunction()


# Add external dependencies easily by calling extern( <action> <optional inputs> ) in the following ways:
# extern( include <dirs> ) to add directories to include
# extern( link <flags> )   to add libraries to link to or other flags
# extern( define <defs> )  to add compiler definitions
# extern( finish )         to combine all external dependencies into a single interface target for other targets to use.
# Only use the finish action once you're done calling any of the others, and you must use it for any of this to work.
# Link to ${TARGET_PREFIX}_interface from other targets to access all external dependencies.
function( extern )
  set( args ${ARGN} )
  list( LENGTH args num_args )
  if ( ${num_args} GREATER 0 )
    list( GET args 0 action )
    if ( action STREQUAL "finish" )
      add_library( ${TARGET_PREFIX}_interface INTERFACE )
      target_include_directories( ${TARGET_PREFIX}_interface INTERFACE SYSTEM
        ${${TARGET_PREFIX}_include}
        )
      target_link_libraries( ${TARGET_PREFIX}_interface INTERFACE
        ${${TARGET_PREFIX}_link}
        )
      target_compile_definitions( ${TARGET_PREFIX}_interface INTERFACE
        ${${TARGET_PREFIX}_define}
        )
      message( STATUS "EXTERN: ${action}: ${TARGET_PREFIX}_interface combines all external dependencies." )
    elseif ( action STREQUAL "include" OR action STREQUAL "link" OR action STREQUAL "define" )
      if ( ${num_args} GREATER 1 )
        list( SUBLIST args 1 -1 inputs )
        set( ${TARGET_PREFIX}_${action} ${${TARGET_PREFIX}_${action}} ${inputs} PARENT_SCOPE )
        message( STATUS "EXTERN: ${action} ${inputs}" )
      else ()
        message( STATUS "EXTERN: ${action} (action received no inputs)" )
      endif ()
    else ()
      message( WARNING "EXTERN: \"${action}\" is not a valid action." )
    endif ()
  else ()
    message( WARNING "EXTERN: No action specified." )
  endif ()
endfunction()


# Call if SDL2 cannot be found, and if it's a Windows build, output a message about how to possibly fix the problem.
function( sdl2_debug_env )
  if ( WIN32 )
    message( WARNING "\
      The system environment variable SDL2 may not be set correctly.
      On Windows, finding SDL2 may require that you do the following:
      1. Download the appropriate SDL2 development libraries from https://www.libsdl.org/download-2.0.php
      2. Unzip/decompress the downloaded archive to get the SDL2-2.#.# directory, where # is some version number.
      3. Place the SDL2-2.#.# directory somewhere permanent. I use C:\\libs, but it doesn't matter where.
      4. Set a system environment variable called SDL2 to point to that directory. For example, in Windows 10,
         a. extract the directory to C:\\libs\\
         b. in the start menu search for \"edit the system environment variables\" and click the matching result
         c. click the \"Environment Variables\" button at the bottom of the small window that appears
         d. in the next window that appears, under the lower pane labelled \"System variables,\" click \"New\"
         e. in the \"Variable name\" field enter \"SDL2\" (without quotes)
         f. in the \"Variable value\" field enter the full path to the SDL2-2.#.# directory or browse to it with
            \"Browse Directory\"
         g. click \"OK\" on each window that's come up so far to apply the changes
      5. Restart any IDE, CLI, or other program you're using to build the CMake project and try again.
    ")
  endif ()
  message( FATAL_ERROR "SDL2 NOT FOUND" )
endfunction()


macro( SUBDIRLIST result curdir )
  file( GLOB children RELATIVE ${curdir} ${curdir}/* )
  set( dirlist "" )
  foreach ( child ${children} )
    if ( IS_DIRECTORY ${curdir}/${child} )
      list( APPEND dirlist ${child} )
    endif ()
  endforeach ()
  set( ${result} ${dirlist} )
endmacro()
