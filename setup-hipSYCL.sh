#!/bin/bash

set -e

git clone https://github.com/illuhad/hipSYCL.git && cd hipSYCL

mkdir -p build && cd build

# if the following does not work, look at this file:
# cat .github/workflows/macos.yml

# to update this, use this:
# brew list --version llvm
export BREW_LLVM=/usr/local/Cellar/llvm/11.0.0

cmake .. -DLLVM_DIR=$BREW_LLVM/lib/cmake/llvm -DCLANG_EXECUTABLE_PATH=$BREW_LLVM/bin/clang++ \
         -DCMAKE_CXX_COMPILER=$BREW_LLVM/bin/clang++ \
         -DCMAKE_CXX_FLAGS="-fopenmp -L$BREW_LLVM/lib -I$BREW_LLVM/include"
make -j 2 VERBOSE=ON
