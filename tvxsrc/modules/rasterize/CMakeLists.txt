
message( "> rasterize:" )

set( LIB_PREFIX ${TARGET_NAME}_rast )

add_library( ${LIB_PREFIX} STATIC
  rast.hpp rast.cpp
  )
target_include_directories( ${LIB_PREFIX} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( ${LIB_PREFIX} PUBLIC ${TARGET_PREFIX}_interface )

add_executable( ${LIB_PREFIX}_test rastTest.cpp )
target_link_libraries( ${LIB_PREFIX}_test ${LIB_PREFIX} )