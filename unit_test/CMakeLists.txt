file ( GLOB UNIT_TEST_SOURCES "*.cpp" )
add_executable ( unit_test ${UNIT_TEST_SOURCES} )
target_link_libraries ( unit_test state_machine )
set_target_properties ( unit_test PROPERTIES CXX_STANDARD 11 )
