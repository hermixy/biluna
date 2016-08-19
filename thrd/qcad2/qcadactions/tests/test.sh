#!/bin/sh

# qcadlib:
cd ../../qcadlib/src
make
# qcadactions:
cd ../../qcadactions/src
make
# tests:
cd ../tests
rm test
qmake
make
./test
