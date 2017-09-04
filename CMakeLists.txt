cmake_minimum_required(VERSION 3.8)
project(CppUtilities)

include(cmake/defaultConfig.cmake           REQUIRED)
include(cmake/findDependencies.cmake        REQUIRED)

add_subdirectory(apps)

include(cmake/printInfo.cmake               REQUIRED)
