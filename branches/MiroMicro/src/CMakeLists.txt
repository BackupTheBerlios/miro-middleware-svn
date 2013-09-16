add_subdirectory( params )
add_subdirectory( makeParams )

add_subdirectory( miro )  # add this after makeParams so we know where to find the executable
if ( RTIDDS_FOUND AND TAO_FOUND )
  add_subdirectory( miroDds )
endif( RTIDDS_FOUND AND TAO_FOUND )
