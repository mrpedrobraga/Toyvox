
message( "> octrender:" )

add_library( ${TARGET_NAME}_octren STATIC
  octren.hpp octren.cpp
  buffers.hpp buffers.cpp
  )
target_include_directories( ${TARGET_NAME}_octren PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( ${TARGET_NAME}_octren PUBLIC ${TARGET_NAME}_sdlc )

add_custom_command(
  TARGET ${TARGET_NAME}_octren POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
  DEPENDS ${destination}
  COMMENT "symbolic link resources folder from ${CMAKE_CURRENT_SOURCE_DIR}/assets => ${CMAKE_CURRENT_BINARY_DIR}/assets"
)

add_executable( ${TARGET_NAME}_octren_test testOctren.cpp )
target_link_libraries( ${TARGET_NAME}_octren_test PRIVATE ${TARGET_NAME}_octren )
