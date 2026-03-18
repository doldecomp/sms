cmake_minimum_required(VERSION 3.8)
project(CapePowerup)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/BetterSunshineEngine)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hxx" "include/*.hpp" "include/*.h")

add_executable(CapePowerup ${SOURCES} ${HEADERS})

target_include_directories(CapePowerup PRIVATE include)
target_compile_options(CapePowerup PRIVATE ${SMS_COMPILE_FLAGS})
target_link_options(CapePowerup PRIVATE ${SMS_LINK_FLAGS})
