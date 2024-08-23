cmake_minimum_required(VERSION 3.30)

set(This tests)

set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/lib/cmake/SDL2" ${CMAKE_PREFIX_PATH})
find_package(SDL2 REQUIRED)

# Define the test executable
set(Sources
    tests.cpp
)

add_executable(${This} ${Sources})

# Link against GoogleTest libraries and your Chess library
target_include_directories(Chess PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/SDL2
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${This} PRIVATE
    Chess
    gtest_main
    SDL2::SDL2
    SDL2::SDL2main
    # ${SDL2_LIBRARIES}
)

set_target_properties(tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

# Register the test with CTest
add_test(
    NAME ${This}
    COMMAND ${This}
)