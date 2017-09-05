cmake_minimum_required(VERSION 3.8)
project(CppUtilities)

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include(cmake/defaultConfig.cmake           REQUIRED)
include(cmake/findDependencies.cmake        REQUIRED)

add_subdirectory(apps)

include(cmake/printInfo.cmake               REQUIRED)
