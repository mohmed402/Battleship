cmake_minimum_required(VERSION 3.14)
project(battleShips)

# Use C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES main.cpp
        BattleshipGameH.cpp
        BattleshipGame.h)
add_executable(battleShips ${SOURCE_FILES})
include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(battleShips sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
