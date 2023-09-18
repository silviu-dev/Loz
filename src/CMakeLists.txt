
add_executable(bootstrap bootstrap.cpp Runner.cpp Runner.hpp)

add_subdirectory(scanner)
add_subdirectory(errorHandling)
add_subdirectory(parser)
add_subdirectory(interpreter)

target_link_libraries(bootstrap PUBLIC scanner errorHandler parser interpreter)

include(Format)
Format(bootstrap .)
include(CppCheck)
AddCppCheck(bootstrap)
