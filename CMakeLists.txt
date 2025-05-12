cmake_minimum_required (VERSION 3.14)

project (recipe VERSION 1.0.0)

option(BUILD_ANDROID "Enable to do android build" OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")
include(utils)

if(${BUILD_ANDROID})
  set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ON)
  find_package(Qt6 COMPONENTS Core Quick Qml Gui REQUIRED)
  find_package(Boost)
  include(android-toolchain)
  add_subdirectory(gui/mobile)
else()
  find_package(Qt6 COMPONENTS Core Quick Qml Gui REQUIRED)
  find_package(Boost)
  find_package(RapidJSON)

  add_subdirectory(recipe)
  add_subdirectory(io)
  add_subdirectory(gui)
  add_subdirectory(communication)
  add_subdirectory(webserver/feeder)
#  add_subdirectory(documentation)

  enable_testing()
  add_subdirectory(test)
endif()

