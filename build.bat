@echo off
mkdir build 2>nul
cd build
cmake ..
cmake --build .
cd ..
cmake --install ./build/ --prefix ./output
pause