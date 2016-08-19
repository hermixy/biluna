#!/bin/bash

cd ..
cwd=`pwd`

for dir in qcadlib qcadcam qcadactions qcadguiqt qcadscripting qcadprop qcad rwbe rwbenetclient rwbearch
do
    if [ -d $dir ]
    then
    	cd $dir
    	$QTDIR/bin/lrelease *.pro
    	cd ..
    
    	cd $dir/src/ts
    	for tf in *.qm
    	do
    		if [ "x$OS" = "xWindows_NT" ]
    		then
    			cp $cwd/$dir/src/ts/$tf $cwd/qcad/qm/$tf
    		else
    			if [ ! -e $cwd/qcad/qm/$tf ]
    			then
    				ln -s $cwd/$dir/src/ts/$tf $cwd/qcad/qm/$tf
    			fi
    		fi
    	done
	    cd ../../..
    fi
done

cd scripts/qt_ts
$QTDIR/bin/lrelease *.ts

for tf in *.qm
do
    if [ "x$OS" = "xWindows_NT" ]
    then
    	cp ./$tf $cwd/qcad/qm/$tf
    else
    	if [ ! -e "$cwd/qcad/qm/$tf" ]
    	then
    		ln -s $cwd/scripts/qt_ts/$tf $cwd/qcad/qm/$tf
    	fi
    fi
done
