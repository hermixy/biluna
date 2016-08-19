#!/bin/sh

cd ..
for dir in qcadlib qcadcam qcadactions qcadguiqt qcadscripting qcadprop qcad rwbe rwbenetclient rwbearch
do
	if [ -d $dir ]
	then
		echo $dir
		cd $dir
		# backup:
		mkdir -p ./ts/last
		cp ./ts/*.ts ./ts/last/
		$QTDIR/bin/lupdate -verbose $dir.pro
		cd ..
	fi
done

