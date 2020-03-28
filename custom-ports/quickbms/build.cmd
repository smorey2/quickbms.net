@echo off
rmdir /S /Q _build
mkdir _build & pushd _build
cmake -G "Ninja" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
popd
cmake --build _build