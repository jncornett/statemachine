cmake_minimum_required ( VERSION 3.5.1 )
project ( Pookie CXX )

add_subdirectory ( pookie )
add_subdirectory ( unit_test )
add_subdirectory ( examples )

set ( CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic"
)
