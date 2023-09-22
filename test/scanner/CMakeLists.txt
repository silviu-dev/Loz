set(TestSourceFiles
ScannerTest.cpp
TokenTest.cpp)
target_sources(unit_tests PRIVATE ${TestSourceFiles})
target_link_libraries(unit_tests PRIVATE scanner)