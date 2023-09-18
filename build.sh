#!/bin/sh
rm src/abstractSyntaxTree/Expr.hpp
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build -t bootstrap