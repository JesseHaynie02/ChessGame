cmake_minimum_required(VERSION 3.30)
project(Chess C CXX)

# Enable coverage flags for GCC/MinGW
# if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
#     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
#     set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
# endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

# Add subdirectory for GoogleTest
add_subdirectory(googletest)

# Define the main application
set(Sources
    src/ChessPiece.cpp
    src/ChessBoard.cpp
    src/ChessGame.cpp
    src/main.cpp
)

add_library(Chess STATIC ${Sources})
target_include_directories(Chess
    PUBLIC ${CMAKE_SOURCE_DIR}/include
)

add_executable(main ${Sources})
target_link_libraries(main Chess)

# Add subdirectory for tests
add_subdirectory(test)