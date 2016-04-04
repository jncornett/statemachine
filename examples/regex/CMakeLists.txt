file ( GLOB REGEX_SOURCES "*.cpp" )
add_executable ( regex ${REGEX_SOURCES} )
target_link_libraries ( regex state_machine )
set_target_properties ( state_machine PROPERTIES CXX_STANDARD 11 )
