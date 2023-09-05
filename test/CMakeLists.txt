include(Testing)

add_executable(loz_test dummyTest.cpp)
target_link_libraries(loz_test PRIVATE addition)
AddTests(loz_test)
EnableCoverage(addition)