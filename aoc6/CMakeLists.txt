cmake_minimum_required (VERSION 3.21)

add_executable(aoc6 aoc6.cpp)
target_include_directories(aoc6 PUBLIC ../common)
target_link_libraries(aoc6 PRIVATE ctre::ctre fmt::fmt)
