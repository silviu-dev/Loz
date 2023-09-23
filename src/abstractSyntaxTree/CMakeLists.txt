set(astPath ${CMAKE_SOURCE_DIR}/src/abstractSyntaxTree)

add_custom_command(OUTPUT ${astPath}/Stmt.hpp
COMMAND mkdir -p tmpDir
COMMAND g++ ${CMAKE_SOURCE_DIR}/generators/TreeGenerator.cpp -o tmpDir/TreeGenerator
COMMAND ./tmpDir/TreeGenerator ${astPath}
COMMAND rm -rf tmpDir
)

add_library(absStatement INTERFACE ${astPath}/Stmt.hpp)
target_include_directories(absStatement INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
