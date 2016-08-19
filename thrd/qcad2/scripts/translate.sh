#!/bin/sh

if [ -z $1 ]
then
	echo "usage ./translate.sh [language id]"
	exit
fi

cd ..
for dir in qcadlib qcadcam qcadactions qcadguiqt qcadscripting qcadprop qcadalgo qcadsvg qcad rwbe rwbenetclient rwbearch
do
    if [ -d $dir/src/ts ]
    then
    	cd $dir/src/ts
    	if [ -f ${dir}_$1.ts ]
    	then
    		$QTDIR/bin/linguist ${dir}_$1.ts
    	fi
    	cd ../../..
    fi
done

