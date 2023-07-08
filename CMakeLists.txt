cmake_minimum_required(VERSION 3.25)
project(Search_engine)

set(CMAKE_CXX_STANDARD 23)
set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(src)
add_subdirectory(nlohmann_json)
add_subdirectory(tests)
