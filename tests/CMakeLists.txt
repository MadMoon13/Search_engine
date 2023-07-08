cmake_minimum_required(VERSION 3.25)

project(Search_engine_tests)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_disable_pthreads ON)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(run_tests
        testSimple.cpp
        ${CMAKE_SOURCE_DIR}/include/invertedIndex.h
        ${CMAKE_SOURCE_DIR}/src/invertedIndex.cpp
        testInvertedIndex.cpp
        ${CMAKE_SOURCE_DIR}/include/searchServer.h
        ${CMAKE_SOURCE_DIR}/src/searchServer.cpp
        testSearchServer.cpp
        )
target_include_directories(run_tests PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(run_tests PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(run_tests PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(run_tests)