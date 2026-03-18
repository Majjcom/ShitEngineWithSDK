@echo off
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -GNinja ..
ninja ShitEditor SEPackage MayGame
cd ..
echo ---------------
echo BUILD FINISH
echo ---------------
PAUSE