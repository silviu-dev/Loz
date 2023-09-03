add_executable(loz main.cpp)

# ... testing and program analysis modules
include(Coverage)
CleanCoverage(loz)

include(Format)
Format(loz .)
include(CppCheck)
AddCppCheck(loz)
