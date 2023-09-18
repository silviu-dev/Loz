add_library(parser STATIC Parser.cpp Parser.hpp)

add_subdirectory(astPrinter)

target_link_libraries(parser PUBLIC astPrinter)

target_include_directories(parser BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/../abstractSyntaxTree
${CMAKE_CURRENT_SOURCE_DIR}/../errorHandling ${CMAKE_CURRENT_SOURCE_DIR}/../scanner)