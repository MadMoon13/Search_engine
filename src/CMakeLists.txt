cmake_minimum_required(VERSION 3.25)



add_executable(search_engine
        main.cpp
        ${CMAKE_SOURCE_DIR}/include/converterJSON.h
        converterJSON.cpp
        ${CMAKE_SOURCE_DIR}/include/invertedIndex.h
        invertedIndex.cpp
        ${CMAKE_SOURCE_DIR}/include/searchServer.h
        searchServer.cpp
        )
target_include_directories(search_engine PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)