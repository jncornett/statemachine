add_library ( state_machine STATIC state_machine.cpp )
set_target_properties ( state_machine PROPERTIES CXX_STANDARD 11 )
target_include_directories ( state_machine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
