cmake_minimum_required (VERSION 3.21)

add_executable(aoc8 aoc8.cpp)
target_include_directories(aoc8 PUBLIC ../common)
target_link_libraries(aoc8 PRIVATE ctre::ctre fmt::fmt)
