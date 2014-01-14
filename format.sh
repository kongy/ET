#! /bin/bash

find *.hpp ! -name symbol.hpp | xargs clang-format -i

clang-format -i *.cpp
