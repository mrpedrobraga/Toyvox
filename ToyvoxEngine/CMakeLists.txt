
add_library( ${TARGET_PREFIX}engine
  sdlc.cpp
  sdlc.h
  )
target_link_libraries( ${TARGET_PREFIX}engine
  ${TARGET_PREFIX}extern_interface
  )
target_include_directories( ${TARGET_PREFIX}engine PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

add_subdirectory( tests )
