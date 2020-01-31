
message( "> sdlcontext:" )

add_library( ${TARGET_NAME}_sdlc STATIC
  extern.hpp
  sdlc.hpp sdlc.cpp
  shader.hpp shader.cpp
  topics.hpp topics.cpp
  )
target_include_directories( ${TARGET_NAME}_sdlc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( ${TARGET_NAME}_sdlc PUBLIC ${TARGET_PREFIX}_extern )

add_executable( ${TARGET_NAME}_test_colors testSdlc.cpp )
target_link_libraries( ${TARGET_NAME}_test_colors PRIVATE ${TARGET_NAME}_sdlc )

add_executable( ${TARGET_NAME}_test_triangle testTriangle.cpp )
target_link_libraries( ${TARGET_NAME}_test_triangle PRIVATE ${TARGET_NAME}_sdlc )