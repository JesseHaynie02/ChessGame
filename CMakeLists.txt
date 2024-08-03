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
set(Headers
    ChessPiece.hpp
    ChessBoard.hpp
    ChessGame.hpp
)
set(Sources
    ChessPiece.cpp
    ChessBoard.cpp
    ChessGame.cpp
)

add_library(Chess STATIC ${Sources} ${Headers})

# Add subdirectory for tests
add_subdirectory(test)