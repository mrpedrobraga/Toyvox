
message( "> octrender:" )
set( TARGET_FINAL ${TARGET_NAME}_octren )
set( TARGET_FINAL_TEST ${TARGET_FINAL}_test )

add_library( ${TARGET_FINAL} STATIC
  buffers.hpp buffers.cpp
  player.hpp player.cpp
  voctree.hpp voctree.cpp
  dynamics.hpp
  )
target_include_directories( ${TARGET_FINAL} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( ${TARGET_FINAL} PUBLIC ${TARGET_NAME}_sdlc )

add_custom_command(
  TARGET ${TARGET_FINAL} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
  DEPENDS ${destination}
  COMMENT "sym-link resources folder from ${CMAKE_CURRENT_SOURCE_DIR}/assets => ${CMAKE_CURRENT_BINARY_DIR}/assets"
)

add_executable( ${TARGET_FINAL_TEST} testOctren.cpp )
target_link_libraries( ${TARGET_FINAL_TEST} PRIVATE ${TARGET_FINAL} )
