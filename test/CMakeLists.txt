include(Testing)

add_executable(unit_tests null.cpp)

add_subdirectory(scanner)

AddTests(unit_tests)
EnableCoverage(addition)