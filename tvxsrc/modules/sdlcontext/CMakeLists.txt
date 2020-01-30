
message( "> sdlcontext:" )

set( LIB_PREFIX ${TARGET_NAME}_sdlc )

add_library( ${LIB_PREFIX} STATIC
  extern.hpp
  sdlc.hpp sdlc.cpp
  shader.hpp shader.cpp
  topics.hpp topics.cpp
  )
target_include_directories( ${LIB_PREFIX} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( ${LIB_PREFIX} PUBLIC ${TARGET_PREFIX}_extern )

add_executable( ${LIB_PREFIX}_test_colors testSdlc.cpp )
target_link_libraries( ${LIB_PREFIX}_test_colors ${LIB_PREFIX} )

add_executable( ${LIB_PREFIX}_test_triangle testTriangle.cpp )
target_link_libraries( ${LIB_PREFIX}_test_triangle ${LIB_PREFIX} )