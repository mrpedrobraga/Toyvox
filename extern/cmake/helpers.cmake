


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



# a helper functions to make adding external libraries cleaner
function( extern action )
  set( args ${ARGN} )
  list( LENGTH args num_args )
  if( ${num_args} GREATER 0 )
    set( ${TARGET_PREFIX}${action} ${${TARGET_PREFIX}${action}} ${args} PARENT_SCOPE)
  else()
    message( WARNING "${action} external: RECEIVED BLANK ARGUMENT")
  endif()
endfunction()
