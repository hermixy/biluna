#-------------------------------------------------
# $Id: acc.pro 1352 2011-01-26 22:01:25Z rutger $
# Project created 2011-04-17
# - for building scan or interviews (SCAN) plugin
#-------------------------------------------------

TEMPLATE    = lib
LANGUAGE    = C++
TARGET      = scan_actionplugin
DESTDIR     = ../lib

CONFIG      += plugin

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
    ../../thrd/nightcharts \
    actions \
    filters \
    gui \
    gui/ui \
    model \
    operations \
    utils

# Carefull with the sequence of the libraries, most basic one last
CONFIG(debug, debug|release) {
    message("Debug libraries used for scan")
    LIBS	+= \
        -L../../bil/debug -ldbd
} else {
    message("Release libraries used for scan")
    LIBS	+= \
        -L../../bil/release -ldb
}

# add forms, headers and sources
exists( ../src/scan.pri ) {
    include( ../src/scan.pri )
} else {
    # message and exit
    error( "An error has occurred, ../src/scan.pri is missing." )
}
#exists( ../../thrd/nightcharts/thrd_chart.pri ) {
#    include( ../../thrd/nightcharts/thrd_chart.pri )
#} else {
#    # message and exit
#    error( "An error has occurred, ../../thrd/nightcharts/thrd_chart.pri is missing." )
#}

# install echoplugin example
#target.path = $$[QT_INSTALL_EXAMPLES]/tools/echoplugin
#sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS echoplugin.pro
#sources.path = $$[QT_INSTALL_EXAMPLES]/tools/echoplugin
#INSTALLS += target sources
#target.path = ../../bil/plugins
#INSTALLS += target
