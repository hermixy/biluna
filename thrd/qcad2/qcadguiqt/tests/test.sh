#!/bin/sh

# qcadlib:
#cd ../../qcadlib/src
#make
# qcadactions:
#cd ../../qcadactions/src
#make
# qcadguiqt:
#cd ../../qcadguiqt/src
#make
# tests:
cd ../tests
rm test
make
./test
