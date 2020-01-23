
set( TARGET_NAME ${TARGET_PREFIX}game )

message( "> tvxsrc:" )

# headers are included as well as source just to aid in IDE parsing (it helps in CLion at least)
add_executable( ${TARGET_NAME}
  components.h
  objects.h
  game.h
  display.h
  tvxcore.h
  tvxutil.h
  any.h
  minigame.cpp
  )
target_link_libraries( ${TARGET_NAME}
  ${TARGET_PREFIX}external
  )