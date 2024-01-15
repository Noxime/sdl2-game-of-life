cmake_minimum_required(VERSION 3.13)
project(GOL)

# add_compile_options(-Wall -Wextra -Wpedantic -g)
add_compile_options(
  "-Wall" "-Wpedantic" "-Wextra" "-fexceptions"
  "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
  "$<$<CONFIG:RELEASE>:-O2;>"
)


file(GLOB SOURCES src/*.cpp)
add_executable(GOL ${SOURCES})
set_property(TARGET GOL PROPERTY CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

target_link_libraries(GOL SDL2::Main SDL2::TTF)
