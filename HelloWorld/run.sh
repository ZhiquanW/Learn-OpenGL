#!/bin/bash         

cd build
rm HelloWorld
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j16
./HelloWorld
