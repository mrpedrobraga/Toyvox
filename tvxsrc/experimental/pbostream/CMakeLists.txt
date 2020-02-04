
message( "> pbostream:" )

add_library( ${TARGET_NAME}_pbos STATIC
  pbos.hpp pbos.cpp
  )
target_include_directories( ${TARGET_NAME}_pbos PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( ${TARGET_NAME}_pbos PUBLIC ${TARGET_NAME}_sdlc )

add_executable( ${TARGET_NAME}_pbos_test testPbos.cpp )
target_link_libraries( ${TARGET_NAME}_pbos_test ${TARGET_NAME}_pbos )