cmake_minimum_required(VERSION 3.25)

project(Game CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CONFIGURATION_TYPES "Debug")

set(CMAKE_BUILD_TYPE Release)

set(HPP resource.h util.h stb_image.h bitmap.h setting.h font.h frame.h graphics.h object.h)
set(CPP impliment.cpp util.cpp setting.cpp font.cpp frame.cpp graphics.cpp object.cpp)

add_executable(game WIN32 main.cpp ${HPP} ${CPP})