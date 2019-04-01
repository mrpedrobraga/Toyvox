
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

#  if ( MSYS OR MINGW )
#  else ()
#  endif ()

  list( GET SDL2_LIBRARY 0 lib )
  get_filename_component( lib_dir ${lib} DIRECTORY )
  set( dll_to_copy ${lib_dir}/SDL2.dll )
  message( STATUS "Looking for ${dll_to_copy}" )

  # Test 32/64 bits, copy dll's to the build folder
#  string( REGEX REPLACE "\\\\" "/" correctDllName "$ENV{SDL2}/lib/x64/SDL2.dll" )


  if ( "${CMAKE_SIZEOF_VOID_P}" EQUAL "8" )
    message( STATUS "Transferring x64 version of SDL2.dll to build tree" )
#    configure_file( ${correctDllName} ${CMAKE_CURRENT_BINARY_DIR}/SDL2.dll COPYONLY )
    configure_file( ${dll_to_copy} ${CMAKE_CURRENT_BINARY_DIR}/SDL2.dll COPYONLY )
  else ( "${CMAKE_SIZEOF_VOID_P}" EQUAL "8" )
    message( STATUS "Transferring x86 version of SDL2.dll to build tree" )
#    configure_file( ${correctDllName} ${CMAKE_CURRENT_BINARY_DIR}/SDL2.dll COPYONLY )
    configure_file( ${dll_to_copy} ${CMAKE_CURRENT_BINARY_DIR}/SDL2.dll COPYONLY )
  endif ( "${CMAKE_SIZEOF_VOID_P}" EQUAL "8" )

endif ( WIN32 )
