add_library(scanner STATIC Scanner.cpp)
target_include_directories(scanner BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})