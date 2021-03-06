#-------------------------------------------------
# $Id: sail.pro 1680 2012-06-23 15:08:20Z rutger $
# Project created 2012-07-26T14:00:00
# - for building sailing plugin SAIL
#-------------------------------------------------

TEMPLATE    = lib
LANGUAGE    = C++
TARGET      = sail_actionplugin

CONFIG      += plugin

# compiler settings for release, debug and some paths
exists( ../../mkspecs/defs.pro ) {
    include( ../../mkspecs/defs.pro )
} else {
    # message and exit
    error( "An error has occurred, ../../mkspecs/defs.pro is missing." )
}

INCLUDEPATH += \
#    ../../qt/qt-enterprise-charts-src-1.3.0/include \
    ../../db/src/actions \
    ../../db/src/entities \
    ../../db/src/gui \
    ../../db/src/gui/ui \
    ../../db/src/model \
    ../../db/src/operations \
    ../../db/src/utils \
    ../../db/src/visitors \
    actions \
    filters \
    gui \
    gui/ui \
    model \
    operations \
    utils \
    visitors

# Carefull with the sequence of the libraries, most basic one last
CONFIG(debug, debug|release) {
    message("Debug libraries used for sail")
    LIBS	+= \
#        -LC:/Qt/5.5/msvc2013/bin/Qt5Chartsd \
#        -L../../rpt/lib -lrptd \
#        -L../../gv/lib -lgvd \
        -L../../bil/debug -ldbd
} else {
    message("Release libraries used for sail")
    LIBS	+= \
#        -LC:/Qt/5.5/msvc2013/bin/Qt5Charts \
#        -L../../rpt/lib -lrpt \
#        -L../../gv/lib -lgv \
        -L../../bil/release -ldb
}

# add forms, headers and sources
exists( ../src/sail.pri ) {
    include( ../src/sail.pri )
} else {
    # message and exit
    error( "An error has occurred, ../src/sail.pri is missing." )
}

