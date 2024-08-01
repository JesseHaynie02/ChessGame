cmake_minimum_required(VERSION 3.30)
project(Chess C CXX)

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