file ( GLOB PROTO_SOURCES "*.cpp" )
add_executable ( proto ${PROTO_SOURCES} )
target_link_libraries ( proto pookie )
