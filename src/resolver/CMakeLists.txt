add_library(resolver STATIC Resolver.cpp)

target_link_libraries(resolver PUBLIC interpreter)

target_include_directories(resolver BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/../errorHandling ${CMAKE_CURRENT_SOURCE_DIR}/../scanner)