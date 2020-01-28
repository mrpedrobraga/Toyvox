
message( "> sdlcontext:" )

set( LIB_PREFIX ${TARGET_NAME}_sdlc )

add_library( ${LIB_PREFIX} STATIC
  extern.hpp
  sdlc.hpp sdlc.cpp
  shader.hpp shader.cpp
  )
target_include_directories( ${LIB_PREFIX} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( ${LIB_PREFIX} PUBLIC ${TARGET_PREFIX}_interface )

add_executable( ${LIB_PREFIX}_test_colors sdlcTest.cpp )
target_link_libraries( ${LIB_PREFIX}_test_colors ${LIB_PREFIX} )

add_executable( ${LIB_PREFIX}_test_triangle triangleTest.cpp )
target_link_libraries( ${LIB_PREFIX}_test_triangle ${LIB_PREFIX} )