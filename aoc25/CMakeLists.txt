cmake_minimum_required (VERSION 3.21)

add_executable(aoc25 aoc25.cpp)
target_include_directories(aoc25 PUBLIC ../common)
target_link_libraries(aoc25 PRIVATE ctre::ctre fmt::fmt)
