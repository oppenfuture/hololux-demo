cmake_minimum_required(VERSION 3.16)
project(hololux_demo VERSION 1.0.0 LANGUAGES C CXX)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

include_directories(dependencies/include)
link_directories(dependencies/lib)

set(SOURCES
  EGLHelper.h
  EGLHelper.cpp
  MineolaHelper.h
  MineolaHelper.cpp
  HololuxScreenshot.h
  HololuxScreenshot.cpp
)

add_executable(buildtest ${SOURCES} HololuxScreenshotBuildTest.cpp)
target_link_libraries(buildtest hololux OpenGL::EGL)

add_library(hololux_demo SHARED ${SOURCES})
target_link_libraries(hololux_demo PUBLIC hololux OpenGL::EGL)

add_custom_command(TARGET hololux_demo
  COMMAND
    ${CMAKE_COMMAND}
    -E
    copy
    ${CMAKE_CURRENT_BINARY_DIR}/libhololux_demo.so
    ${CMAKE_CURRENT_SOURCE_DIR}/artifact
  COMMENT "Copy build artifact"
)

enable_testing()

add_executable(egltest EGLHelperTest.cpp)
target_link_libraries(egltest hololux_demo)
add_test(egltest egltest)

add_executable(mineolatest MineolaHelperTest.cpp)
target_link_libraries(mineolatest hololux_demo)
add_test(mineolatest mineolatest)

add_executable(apptest HololuxScreenshotTest.cpp)
target_link_libraries(apptest hololux_demo imgpp turbojpeg png16 z)
add_custom_command(TARGET apptest
  COMMAND
    ${CMAKE_COMMAND}
    -E
    copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/data
    ${CMAKE_CURRENT_BINARY_DIR}/data
  COMMENT "Copy test data to build folder"
)
add_test(apptest apptest)
