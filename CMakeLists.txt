cmake_minimum_required(VERSION 3.7)
project(WorldsBestTextEditor)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE DEBUG)

#set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_MODULE_PATH /home/ecb/Worlds-Best-Text-Editor)

find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
include_directories(WorldsBestTextEditor include ${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIR})

set(SOURCE_FILES src/main.cpp)

add_executable(WorldsBestTextEditor ${SOURCE_FILES})
target_link_libraries(WorldsBestTextEditor ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARY})
