#*****************************************************************
# $Id: build_biluna.sh - 21-Dec-2007 10:06:52 PM - rutger $
#
# Copyright (C) 2007 Red-Bag. All rights reserved.
# This file is part of the Biluna 'scripts' project.
#
# See http://www.red-bag.com for further details.
#*****************************************************************

# script to build all Biluna projects

#!/bin/bash

echo "--- building Biluna ---"

if [ "x$OS" == "xWindows_NT" ]
then
    export MAKE="MinGW32-make -j2"
    echo "Platform is Windows"
    QMAKE_OPT="$QMAKE_OPT QMAKE_DEL_FILE=rm"
elif [ `uname` == "SunOS" ]
then
    export MAKE=gmake
    echo "Platform is Solaris"
elif [ x`echo $OSTYPE | sed s/[0-9.]*//g` == "xdarwin" ]
then
    export MAKE="make -j2 CXX=\"ccache gcc\""
    echo "Platform is Mac OS X"
else
    export MAKE="make -j4 CXX=\"ccache gcc\""
    echo "Platform is Linux"
fi

cd ..

# libs are: db te gl ves pcc job
# executables are: clp is a terminal application 
# and others are gui applications
# Tests applications are not build

for p in db acc crm mrp pcalc peng sail scan srm bil
do
	if [ -d $p ]
	then
		echo "--- building $p ---"
		cd $p/src
#        if [ $p == "qcad" ]
#        then
#            rm qcad
        fi
        if [ $p.pro -nt Makefile ]
        then
#            qmake CONFIG+=rs_qcad
	    qmake
        fi
		eval $MAKE
        if [ $? != "0" ]
        then
            exit $?
        fi
        echo "--- end $p build ---"
		cd ../..
	fi
done
