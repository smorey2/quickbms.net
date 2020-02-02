@echo off
rmdir /S /Q _vcpkg\buildtrees\quickbms\src\0.10.1-6971604e98\_build
mkdir _vcpkg\buildtrees\quickbms\src\0.10.1-6971604e98\_build & pushd _vcpkg\buildtrees\quickbms\src\0.10.1-6971604e98\_build
cmake -G "Ninja" -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ ..
popd
cmake --build _vcpkg\buildtrees\quickbms\src\0.10.1-6971604e98\_build