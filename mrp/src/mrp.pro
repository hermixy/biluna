#-------------------------------------------------
# $Id: mrp.pro 1680 2012-06-23 15:08:20Z rutger $
# Project created 2014-10-23T14:00:00
# - for building material requirements planning or
#   manufacturing resource planning plugin MRP
#-------------------------------------------------

TEMPLATE    = lib
LANGUAGE    = C++
TARGET      = mrp_actionplugin

CONFIG      += qt plugin warn_on
QT          += core network sql webkit xml

# compiler settings for release, debug and some paths
exists( ../../mkspecs/defs.pro ) {
    include( ../../mkspecs/defs.pro )
} else {
    # message and exit
    error( "An error has occurred, ../../mkspecs/defs.pro is missing." )
}

INCLUDEPATH += \
    ../../acc/src/model \
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
    message("Debug libraries used for mrp")
    LIBS	+= \
#        -L../../gv/lib -lgvd \
        -L../../bil/debug -ldbd
} else {
    message("Release libraries used for mrp")
    LIBS	+= \
#        -L../../gv/lib -lgv \
        -L../../bil/release -ldb
}

# add forms, headers and sources
exists( ../src/mrp.pri ) {
    include( ../src/mrp.pri )
} else {
    # message and exit
    error( "An error has occurred, ../src/mrp.pri is missing." )
}

