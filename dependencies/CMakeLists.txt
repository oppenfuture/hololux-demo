cmake_minimum_required(VERSION 3.16)
project(hololux VERSION 1.1.0 LANGUAGES C CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_compile_definitions(WOLF_DISABLE_BASISU)

file(GLOB WOLF_SRC ../../../wolf/src/*.cpp)
file(GLOB WOLF_HDR ../../../wolf/include/*.h)
add_library(hololux SHARED ${WOLF_SRC} ${WOLF_HDR})
target_compile_features(hololux PUBLIC cxx_std_17)
target_include_directories(hololux PUBLIC ../../include)
target_link_libraries(hololux PUBLIC
  CONAN_PKG::rapidjson
  CONAN_PKG::mineola
  CONAN_PKG::lz4
)
