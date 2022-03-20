#!/bin/bash         

cd build
rm HelloWorld
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j8
./HelloWorld