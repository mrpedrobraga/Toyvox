
set( TARGET_NAME ${TARGET_PREFIX}test_ )

add_executable( ${TARGET_NAME}objects
  objects.cpp
  )
target_link_libraries( ${TARGET_NAME}objects
  ${TARGET_PREFIX}engine
  )

add_executable( ${TARGET_NAME}graphics
  graphics.cpp
  )
target_link_libraries( ${TARGET_NAME}graphics
  ${TARGET_PREFIX}engine
  )

if ( WIN32 )
  if ( MSYS OR MINGW ) # Better to not check these together?
    set( dll_to_copy $ENV{MSYSTEM_PREFIX}/bin/SDL2.dll )
    message( STATUS "Looking for ${dll_to_copy}" )
    configure_file( ${dll_to_copy} ${CMAKE_CURRENT_BINARY_DIR}/SDL2.dll COPYONLY )
    message( STATUS "Copied SDL2.dll to build tree." )
  else () # Assume a VC/MSVC environment
    # The SDL2-2.x.x directory for this platform should contain the dll in the same place as the static libs
    list( GET SDL2_LIBRARY 0 lib ) # so just take any static lib
    get_filename_component( lib_dir ${lib} DIRECTORY ) # and get the directory from its path
    set( dll_to_copy ${lib_dir}/SDL2.dll )
    message( STATUS "Looking for ${dll_to_copy}" )
    configure_file( ${dll_to_copy} ${CMAKE_CURRENT_BINARY_DIR}/SDL2.dll COPYONLY )
    message( STATUS "Copied SDL2.dll to build tree." )
  endif ()
endif ( WIN32 )
