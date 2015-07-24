#-------------------------------------------------
# $Id: srm.pro 1680 2012-06-23 15:08:20Z rutger $
# Project created 2013-02-21T14:11:30
# - for building supplier relation management plugin SRM
#-------------------------------------------------

TEMPLATE    = lib
LANGUAGE    = C++
TARGET      = srm_actionplugin

CONFIG      += qt plugin warn_on
#QT          += core network sql webkit xml

# compiler settings for release, debug and some paths
exists( ../../mkspecs/defs.pro ) {
    include( ../../mkspecs/defs.pro )
} else {
    # message and exit
    error( "An error has occurred, ../../mkspecs/defs.pro is missing." )
}

INCLUDEPATH += \
    ../../db/src/actions \
    ../../db/src/entities \
    ../../db/src/gui \
    ../../db/src/gui/ui \
    ../../db/src/model \
    ../../db/src/operations \
    ../../db/src/utils \
    ../../db/src/visitors \
    ../../acc/src/actions \
    ../../acc/src/gui \
    ../../acc/src/gui/ui \
    ../../acc/src/model \
    ../../acc/src/utilities \
    ../../acc/src/visitors \
    actions \
    gui \
    model \
    operations \
    utils \
    visitors

# Carefull with the sequence of the libraries, most basic one last
CONFIG(debug, debug|release) {
    message("Debug libraries used for srm")
    LIBS	+= \
#        -L../../rpt/lib -lrptd \
#        -L../../gv/lib -lgvd \
        -L../../bil/debug -ldbd
} else {
    message("Release libraries used for srm")
    LIBS	+= \
#        -L../../rpt/lib -lrpt \
#        -L../../gv/lib -lgv \
        -L../../bil/release -ldb
}

# add forms, headers and sources
exists( ../src/srm.pri ) {
    include( ../src/srm.pri )
} else {
    # message and exit
    error( "An error has occurred, ../src/srm.pri is missing." )
}
