cmake_minimum_required(VERSION 3.9)
project(mementar_btree)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/CompileWarnings.cmake)
include(cmake/StaticAnalyzers.cmake)

add_executable(btree_test test/btree_test.cpp)
target_include_directories(btree_test PRIVATE include)
target_compile_features(btree_test PRIVATE cxx_std_17)
target_compile_warnings(btree_test WARNINGS_AS_ERRORS FATAL_ERRORS)