set(astPath ${CMAKE_SOURCE_DIR}/src/abstractSyntaxTree)

add_custom_command(OUTPUT ${astPath}/Expr.hpp
COMMAND mkdir -p tmpDir
COMMAND g++ ${CMAKE_SOURCE_DIR}/generators/TreeGenerator.cpp -o tmpDir/TreeGenerator
COMMAND ./tmpDir/TreeGenerator ${astPath}
COMMAND rm -rf tmpDir
)

add_library(astPrinter STATIC AstPrinter.cpp ${astPath}/Expr.hpp)
target_include_directories(astPrinter BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
${astPath})
