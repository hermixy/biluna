#-------------------------------------------------
# $Id: crm.pro 2248 2015-06-21 09:13:00Z rutger $
# Project created 2012-02-12T14:11:30
# - for building customer relation management plugin CRM
#-------------------------------------------------

TEMPLATE    = lib
LANGUAGE    = C++
TARGET      = crm_actionplugin

CONFIG      += plugin
#QT          +=

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
    message("Debug libraries used for crm")
    LIBS	+= \
        -L../../bil/debug -ldbd
} else {
    message("Release libraries used for crm")
    LIBS	+= \
        -L../../bil/release -ldb
}

# add forms, headers and sources
exists( ../src/crm.pri ) {
    include( ../src/crm.pri )
} else {
    # message and exit
    error( "An error has occurred, ../src/crm.pri is missing." )
}
