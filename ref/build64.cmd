@echo off
rmdir /S /Q _build64
mkdir _build64 & pushd _build64
cmake -G "Visual Studio 15 2017 Win64" ..
popd
cmake --build _build64