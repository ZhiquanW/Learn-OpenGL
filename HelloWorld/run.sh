#!/bin/bash         

cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j8
./HelloWorld