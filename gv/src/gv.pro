# ----------------------------------------------------------
# $Id: gv.pro 1108 2010-03-08 21:49:30Z rutger $
# gv.pro 2008-03-12 rutger
# - for building gv lib
# ----------------------------------------------------------

TEMPLATE    = lib
LANGUAGE    = C++
TARGET      = gv

# Compiler settings for release, debug and some paths
exists( ../../mkspecs/defs.pro ) {
    include( ../../mkspecs/defs.pro )
} else {
    # message and exit
    error( "An error has occurred, ../../mkspecs/defs.pro is missing." )
}

CONFIG      += qt lib warn_on
QT          += core gui network opengl script scripttools sql svg webkit xml


INCLUDEPATH += \
    ../../db/src/actions \
    ../../db/src/entities \
    ../../db/src/gui \
    ../../db/src/gui/ui \
    ../../db/src/model \
    ../../db/src/operations \
    ../../db/src/utils \
    ../../db/src/visitors \
#    ../../gl/src/model \
#    ../../ldt/src/gui \
#    ../../ldt/src/model \
    ../../thrd/qt-solutions/qtpropertybrowser/src \
    actions \
    core \
    entity \
    filters \
    grid \
    gui \
    gui/qt \
    gui/qt/qgraphicsview \
    gui/ui \
    math \
    model \
    operations \
    snap
    # spatialindexnavel

# Carefull with the sequence of the libraries, most basic one last
CONFIG(debug, debug|release) {
    message("Debug libraries used for gv")
    LIBS	+= \
        -L../../bil/debug -ldbd
} else {
    message("Release libraries used for gv")
    LIBS	+= \
        -L../../bil/release -ldb
}

# add forms, headers and sources. NOTE sequence order is important
include( ../src/math.pri )
include( ../src/entity.pri )
include( ../src/grid.pri )
include( ../src/snap.pri )
include( ../src/operations.pri )
# include( ../src/spatialindexnavel.pri )
include( ../src/core.pri )
include( ../src/gui.pri )
include( ../src/gv.pri )
include( ../../thrd/qt-solutions/qtpropertybrowser/qtpropertybrowser.pri )

#FORMS += \
#    gui/gv_mainwindow_old.ui

#HEADERS	+= \
#    ../src/gui/gv_mainwindow.h \
#    ../src/gui/gv_mainwindow_old.h \
#    ../src/gui/main.h

#SOURCES	+= \
#    ../src/gui/gv_mainwindow.cpp \
#    ../src/gui/gv_mainwindow_old.cpp \
#    ../src/gui/main.cpp


