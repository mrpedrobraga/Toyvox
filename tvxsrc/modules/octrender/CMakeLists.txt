
message( "> octrender:" )

set( LIB_PREFIX ${TARGET_NAME}_octren )

add_library( ${LIB_PREFIX} STATIC
  octren.hpp octren.cpp
  )
target_include_directories( ${LIB_PREFIX} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( ${LIB_PREFIX} PUBLIC ${TARGET_PREFIX}_extern )

add_custom_command(
  TARGET ${LIB_PREFIX} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
  DEPENDS ${destination}
  COMMENT "symbolic link resources folder from ${CMAKE_CURRENT_SOURCE_DIR}/assets => ${CMAKE_CURRENT_BINARY_DIR}/assets"
)

add_executable( ${LIB_PREFIX}_test testOctren.cpp )
target_link_libraries( ${LIB_PREFIX}_test ${LIB_PREFIX} ${TARGET_NAME}_sdlc )
