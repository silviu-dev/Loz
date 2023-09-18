include(Testing)

add_executable(unit_tests null.cpp)
target_include_directories(unit_tests BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mocks)

add_subdirectory(scanner)
add_subdirectory(errorHandling)
add_subdirectory(parser)

AddTests(unit_tests)
EnableCoverage(errorHandler)
EnableCoverage(scanner)
EnableCoverage(parser)