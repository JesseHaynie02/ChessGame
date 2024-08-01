cmake_minimum_required(VERSION 3.30)

set(This tests)

# Define the test executable
set(Sources
    tests.cpp
)

add_executable(${This} ${Sources})

# Link against GoogleTest libraries and your Chess library
target_link_libraries(${This} PRIVATE
    gtest_main
    Chess
)

# Register the test with CTest
add_test(
    NAME ${This}
    COMMAND ${This}
)