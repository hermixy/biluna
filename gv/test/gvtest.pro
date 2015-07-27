# ----------------------------------------------------------
# $Id: gv.pro 1108 2010-03-08 21:49:30Z rutger $
# gv.pro 2008-03-12 rutger
# - for building gv application
# ----------------------------------------------------------

TEMPLATE    = app
LANGUAGE    = C++
TARGET      = gvtest

# Compiler settings for release, debug and some paths
exists( ../../mkspecs/defstest.pro ) {
    include( ../../mkspecs/defstest.pro )
} else {
    # message and exit
    error( "An error has occurred, ../../mkspecs/defstest.pro is missing." )
}

# Override destination path
#DESTDIR     = ./

CONFIG      += qt warn_on
QT          += \
    concurrent \
    network \
    opengl \
    script \
    scripttools \
    sql \
    svg \
    webkit \
    webkitwidgets \
    xml

INCLUDEPATH += \
    ../../db/src/actions \
    ../../db/src/gui \
    ../../db/src/gui/ui \
    ../../db/src/model \
    ../../db/src/utils \
    ../../db/src/visitors \
    ../../thrd/qtsingleapplication/src \
    ../src/actions \
    ../src/core \
    ../src/entity \
    ../src/filters \
    ../src/grid \
    ../src/gui \
    ../src/gui/qt \
    ../src/gui/qt/qgraphicsview \
    ../src/gui/ui \
    ../src/math \
    ../src/model \
    ../src/operations \
    ../src/snap
#    ../src/spatialindexnavel \
#    ../src/spatialindexnavel/spatialindex \
#    ../src/spatialindexnavel/spatialindex/include \
#    ../src/spatialindexnavel/spatialindex/include/tools \
#    ../src/spatialindexnavel/spatialindex/src/mvrtree \
#    ../src/spatialindexnavel/spatialindex/src/rtree \
#    ../src/spatialindexnavel/spatialindex/src/spatialindex \
#    ../src/spatialindexnavel/spatialindex/src/storagemanager \
#    ../src/spatialindexnavel/spatialindex/src/tools \
#    ../src/spatialindexnavel/spatialindex/src/tprtree


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
#include( ../src/spatialindexnavel.pri )
include( ../src/core.pri )
include( ../src/gui.pri )
include( ../src/gv.pri )
include( ../../thrd/qt-solutions/qtpropertybrowser/qtpropertybrowser.pri )


FORMS += \
#    gui/gv_mainwindow_old.ui

HEADERS	+= \
    gv_mainwindow.h \
    main.h

SOURCES	+= \
    gv_mainwindow.cpp \
    main.cpp
