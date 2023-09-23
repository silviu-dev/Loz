#!/bin/sh
rm src/abstractSyntaxTree/Expr.hpp
rm src/abstractSyntaxTree/Stmt.hpp
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build -t bootstrap