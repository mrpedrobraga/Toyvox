
message( "> rasterize:" )

add_library( ${TARGET_NAME}_rast STATIC
  rast.hpp rast.cpp
  )
target_include_directories( ${TARGET_NAME}_rast PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( ${TARGET_NAME}_rast PUBLIC ${TARGET_PREFIX}_extern )

add_executable( ${TARGET_NAME}_rast_test testRast.cpp )
target_link_libraries( ${TARGET_NAME}_rast_test ${TARGET_NAME}_rast )