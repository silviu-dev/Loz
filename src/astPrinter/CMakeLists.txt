cmake_minimum_required(VERSION 3.20.0)
project(Loz VERSION 1.0.0 LANGUAGES CXX)

add_library(astPrinter STATIC AstPrinter.cpp)
target_include_directories(astPrinter BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
../abstractSyntaxTree)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC astPrinter)