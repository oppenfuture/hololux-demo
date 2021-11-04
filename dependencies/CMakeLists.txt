cmake_minimum_required(VERSION 3.16)
project(hololux VERSION 1.1.0 LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})

if (EXISTS ${CMAKE_BINARY_DIR}/conan_paths.cmake)
  include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
endif()

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

find_package(mineola REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(lz4 REQUIRED)

file(GLOB WOLF_SRC ../../../wolf/src/*.cpp)
set(WOLF_SRC ${WOLF_SRC} ../../../third_party/include/transcoder/basisu_transcoder.cpp)
file(GLOB WOLF_HDR ../../../wolf/include/*.h)
add_library(hololux SHARED ${WOLF_SRC} ${WOLF_HDR})
target_compile_features(hololux PUBLIC cxx_std_17)
target_include_directories(hololux PUBLIC ../../include)
target_link_libraries(hololux PUBLIC
  RapidJSON::RapidJSON
  mineola::mineola
  lz4::lz4
  libjpeg-turbo::libjpeg-turbo
)
