
message( "> sdlcontext:" )

add_library( ${TARGET_NAME}_sdlc STATIC
  sdlc.hpp sdlc.cpp
  )
target_include_directories( ${TARGET_NAME}_sdlc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( ${TARGET_NAME}_sdlc PUBLIC ${TARGET_PREFIX}_interface )

add_executable( ${TARGET_NAME}_sdlc_test test.cpp )
target_link_libraries( ${TARGET_NAME}_sdlc_test ${TARGET_NAME}_sdlc )