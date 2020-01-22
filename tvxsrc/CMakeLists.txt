
set( TARGET_NAME ${TARGET_PREFIX}game )

message( "> tvxsrc:" )

# headers are included as well as source just to aid in IDE parsing (it helps in CLion at least)
add_executable( ${TARGET_NAME}
  tester.cpp
  components.h
  objects.h
  game.h
  tvxcore.h
  tvxutil.h
  any.h
  )
target_link_libraries( ${TARGET_NAME}
  ${TARGET_PREFIX}external
  )
