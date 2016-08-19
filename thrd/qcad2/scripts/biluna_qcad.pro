#*****************************************************************
# $Id: biluna_qcad.pro 857 2009-01-02 12:42:09Z rutger $
# Created: 2009-01-02 for building qcad with MingW for biluna
#
# Copyright (C) 2009 Red-Bag. All rights reserved.
# This file is part of the Biluna project.
#
# See http://www.red-bag.com for further details.
#*****************************************************************

TEMPLATE = subdirs

SUBDIRS = \
	../dxflib \
	../fparser \
#	../mkspecs \
        ../qcadlib \
        ../qcadguiqt \
        ../qcadprop \
        ../qcadactions \
        ../qcad
#	../qcadsvg
