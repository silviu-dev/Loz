add_executable(bootstrap bootstrap.cpp Runner.cpp Runner.hpp)

#add_library(addition STATIC logic.cpp)
#target_include_directories(addition SYSTEM BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# ... testing and program analysis modules
#include(Coverage)
#CleanCoverage(addition)

add_subdirectory(scanner)

target_link_libraries(bootstrap PRIVATE scanner)

include(Format)
Format(bootstrap .)
include(CppCheck)
AddCppCheck(bootstrap)
