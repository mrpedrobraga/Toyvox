
message( "> sdlcontext:" )

set( LIB_PREFIX ${TARGET_NAME}_sdlc )

add_library( ${LIB_PREFIX} STATIC
  sdlc.hpp sdlc.cpp
  )
target_include_directories( ${LIB_PREFIX} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( ${LIB_PREFIX} PUBLIC ${TARGET_PREFIX}_interface )

add_executable( ${LIB_PREFIX}_test sdlcTest.cpp )
target_link_libraries( ${LIB_PREFIX}_test ${LIB_PREFIX} )