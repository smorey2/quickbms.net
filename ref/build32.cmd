@echo off
rmdir /S /Q _build32
mkdir _build32 & pushd _build32
cmake -G "Visual Studio 15 2017" ..
popd
cmake --build _build32