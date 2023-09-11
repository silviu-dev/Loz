#!/bin/sh
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build -t memcheck-unit_tests
