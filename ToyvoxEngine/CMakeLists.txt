
# headers are included as well as source just to aid in IDE parsing (it helps in CLion at least)

set( TARGET_NAME ${TARGET_PREFIX}test_ )

add_executable( ${TARGET_NAME}objects
  events.h
  objects.h
  objectsTest.cpp
  tvxcore.h
  )
target_link_libraries( ${TARGET_NAME}objects
  ${TARGET_PREFIX}external
  )

add_executable( ${TARGET_NAME}sdl2
  sdlc.cpp
  sdlc.h
  sdlTest.cpp
  )
target_link_libraries( ${TARGET_NAME}sdl2
  ${TARGET_PREFIX}external
  )

