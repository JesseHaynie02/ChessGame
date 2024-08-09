cmake_minimum_required(VERSION 3.30)
project(Chess C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

# Add subdirectory for GoogleTest
add_subdirectory(googletest)

# Set the path to the SDL2 CMake configuration
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/lib/cmake/SDL2" ${CMAKE_PREFIX_PATH})
find_package(SDL2 REQUIRED)

# Define the main application
set(Sources
    src/ChessPiece.cpp
    src/ChessBoard.cpp
    src/ChessGame.cpp
    src/ChessGraphic.cpp
    src/main.cpp
)

add_library(Chess STATIC ${Sources})
target_include_directories(Chess PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/SDL2
    ${SDL2_INCLUDE_DIRS}
)

add_executable(main ${Sources})

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(main PRIVATE
    Chess
    ${SDL2_LIBRARIES}
)

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

# Add subdirectory for tests
add_subdirectory(test)