add_library ( pookie STATIC pookie.cpp )
set_target_properties ( pookie PROPERTIES CXX_STANDARD 11 )
target_include_directories ( pookie PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
