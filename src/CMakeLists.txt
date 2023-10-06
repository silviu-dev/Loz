
add_executable(bootstrap bootstrap.cpp Runner.cpp Runner.hpp)

add_subdirectory(abstractSyntaxTree)
add_subdirectory(scanner)
add_subdirectory(parser)
add_subdirectory(interpreter)
add_subdirectory(resolver)
add_subdirectory(errorHandling)

target_link_libraries(bootstrap PUBLIC scanner parser resolver errorHandler)

include(Format)
Format(bootstrap .)
include(CppCheck)
AddCppCheck(bootstrap)
