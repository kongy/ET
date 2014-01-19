#! /bin/bash

# Work around for clang-format bug
find *.hpp ! -name symbol.hpp | xargs clang-format -i

clang-format -i *.cpp
