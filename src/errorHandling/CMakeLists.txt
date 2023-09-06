add_library(errorHandler STATIC ErrorHandler.cpp)
target_include_directories(errorHandler BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})