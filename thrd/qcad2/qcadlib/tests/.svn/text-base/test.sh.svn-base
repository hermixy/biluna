#!/bin/sh

# qcadlib:
cd ../src
make -j8
if [ $? -eq 0 ]
then
    # tests:
    cd ../tests
    rm test
    qmake CONFIG+=rs_qcad
    make
    if [ $? -eq 0 ]
    then
        ./test 2>&1 | grep -i equi
    fi
fi
