
message( "> octrender:" )

add_library( ${TARGET_NAME}_octren STATIC
  octren.hpp octren.cpp
  )
target_include_directories( ${TARGET_NAME}_octren PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( ${TARGET_NAME}_octren PUBLIC ${TARGET_PREFIX}_interface )

add_executable( ${TARGET_NAME}_octren_test test.cpp )
target_link_libraries( ${TARGET_NAME}_octren_test ${TARGET_NAME}_octren )