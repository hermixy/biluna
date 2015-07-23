#*****************************************************************
# $Id: clean_biluna.sh - 21-Dec-2007 10:16:52 PM - rutger $
#
# Copyright (C) 2007 Red-Bag. All rights reserved.
# This file is part of the Biluna 'scripts' project.
#
# See http://www.red-bag.com for further details.
#*****************************************************************

# script to clean all Biluna projects

#!/bin/bash

echo "Cleaning auth db te gl veslib pcc auth job ldt eql gv ecc ..."

if [ "x$OS" == "xWindows_NT" ]
then
    export MAKE=MinGW32-make
    echo "Platform is Windows"
    QMAKE_OPT="$QMAKE_OPT QMAKE_DEL_FILE=rm"
elif [ `uname` == "SunOS" ]
then
    export MAKE=gmake
    echo "Platform is Solaris"
elif [ x`echo $OSTYPE | sed s/[0-9.]*//g` == "xdarwin" ]
then
    export MAKE=make
    echo "Platform is Mac OS X"
else
    export MAKE=make
    echo "Platform is Linux"
fi

cd ..

# libs are: db te gl ves pcc job
# executables are: clp is a terminal application 
# and others are gui applications

for p in db te gl clp hlp ves pcc auth job frm web cad ldt eql gv ecc
do
	if [ -d $p ]
	then
		echo "--- cleaning $p ---"
		cd $p/src
		#qmake $p.pro $QMAKE_OPT
		eval $MAKE clean
		if [ -d ../test ]
		then
			eval $MAKE clean
		fi
		cd ../..
	fi
done
