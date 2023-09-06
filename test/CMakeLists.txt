include(Testing)

add_executable(unit_tests null.cpp)

add_subdirectory(scanner)
add_subdirectory(errorHandling)

AddTests(unit_tests)
EnableCoverage(addition)