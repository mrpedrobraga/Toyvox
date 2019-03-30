
# headers are included as well as source just to aid in IDE parsing (it helps in CLion at least)

set( TARGET_NAME ${TARGET_PREFIX}test_ )

add_executable( ${TARGET_NAME}objects
  events.h
  objects.h
  objectsTest.cpp
  tvxcore.h
  )
target_link_libraries( ${TARGET_NAME}objects
  ${TARGET_PREFIX}extern_all
  )

add_executable( ${TARGET_NAME}sdl2
  sdlc.cpp
  sdlc.h
  sdlTest.cpp
  )
target_link_libraries( ${TARGET_NAME}sdl2
  ${TARGET_PREFIX}extern_all
  )

if ( WIN32 )

  # Test 32/64 bits, copy dll's to the build folder
  string( REGEX REPLACE "\\\\" "/" correctDllName "$ENV{SDL2}/lib/x64/SDL2.dll" )
  if ( "${CMAKE_SIZEOF_VOID_P}" EQUAL "8" )
    message( STATUS "Transferring x64 version of SDL2.dll to build tree" )
    configure_file( ${correctDllName} ${CMAKE_CURRENT_BINARY_DIR}/SDL2.dll COPYONLY )
  else ( "${CMAKE_SIZEOF_VOID_P}" EQUAL "8" )
    message( STATUS "Transferring x86 version of SDL2.dll to build tree" )
    configure_file( ${correctDllName} ${CMAKE_CURRENT_BINARY_DIR}/SDL2.dll COPYONLY )
  endif ( "${CMAKE_SIZEOF_VOID_P}" EQUAL "8" )

endif ( WIN32 )
