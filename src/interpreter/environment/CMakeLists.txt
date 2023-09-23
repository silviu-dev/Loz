add_library(environment STATIC Environment.cpp)
target_include_directories(environment BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/../../scanner)