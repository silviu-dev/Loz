add_library(scanner STATIC Scanner.cpp IScanner.hpp)
target_include_directories(scanner BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/../errorHandling)