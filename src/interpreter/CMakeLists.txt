add_subdirectory(environment)

add_library(interpreter STATIC Interpreter.cpp RuntimeFunction.cpp RuntimeClass.cpp RuntimeClassInstance.cpp)
target_link_libraries(interpreter PUBLIC absStatement environment)
target_include_directories(interpreter BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/../errorHandling
${CMAKE_CURRENT_SOURCE_DIR}/../scanner)
