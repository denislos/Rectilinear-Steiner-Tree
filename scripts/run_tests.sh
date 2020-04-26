#! /bin/bash

cd ..
mkdir build
cd build && cmake .. && make


for test_file in $(ls ../tests | egrep -i '*_[0-9]+.xml')
do
    printf "\n\nTest file: $test_file"
    time ./steiner ../tests/$test_file
done
