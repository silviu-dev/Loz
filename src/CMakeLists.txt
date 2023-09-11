
set(absPath ${CMAKE_SOURCE_DIR}/src/abstractSyntaxTree)

add_custom_command(OUTPUT ${absPath}/Expr.hpp
COMMAND mkdir tmpDir
COMMAND g++ ${CMAKE_SOURCE_DIR}/generators/TreeGenerator.cpp -o tmpDir/TreeGenerator
COMMAND ./tmpDir/TreeGenerator ${absPath}
COMMAND rm -rf tmpDir
)

add_executable(bootstrap bootstrap.cpp Runner.cpp Runner.hpp ${absPath}/Expr.hpp)

add_subdirectory(scanner)
add_subdirectory(errorHandling)

target_link_libraries(bootstrap PUBLIC scanner errorHandler)

include(Format)
Format(bootstrap .)
include(CppCheck)
AddCppCheck(bootstrap)
