CMAKE_MINIMUM_REQUIRED(VERSION 3.7)

#SET(CMAKE_CXX_STANDARD 11)

PROJECT(WorldsBestTextEditor)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_BUILD_TYPE DEBUG)

#SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

SET(CMAKE_MODULE_PATH /home/ecb/Worlds-Best-Text-Editor)

FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(SDL2TTF REQUIRED)
INCLUDE_DIRECTORIES(WorldsBestTextEditor include ${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIR})
INCLUDE_DIRECTORIES(~/Program-Arguments)

SET(SOURCE_FILES src/main.cpp src/Functions.cpp src/FunctionCallback.cpp src/GUIObject.cpp src/Buffer.cpp src/Textbox.cpp src/Textbox_Draw.cpp src/Textbox_Scroll.cpp src/Textbox_Cursor.cpp src/Actions.cpp)

ADD_EXECUTABLE(WorldsBestTextEditor ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(WorldsBestTextEditor ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARY})
