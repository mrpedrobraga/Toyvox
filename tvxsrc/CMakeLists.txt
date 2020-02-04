
message( "> tvxsrc:" )

set( TARGET_NAME ${TARGET_PREFIX}_experiment )

add_subdirectory( experimental ) # Targets built in here will have names starting with ${TARGET_PREFIX}_module

set( TARGET_NAME ${TARGET_PREFIX}_game )

# Targets built out here will have names starting with ${TARGET_PREFIX}_game
