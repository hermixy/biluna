#-------------------------------------------------
# $Id: acc.pro 2055 2013-12-18 11:20:20Z rutger $
# Project created by QtCreator 2009-12-18T18:11:30
# - for building accounting plugin ACC
#-------------------------------------------------

TEMPLATE    = lib
LANGUAGE    = C++
TARGET      = acc_actionplugin

CONFIG      += plugin

DEFINES  += ACC_ACTIONPLUGIN_LIBRARY

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
    ../../thrd/nightcharts \
    actions \
    filters \
    gui \
    model \
    operations \
    utils \
    visitors

# Carefull with the sequence of the libraries, most basic one last
CONFIG(debug, debug|release) {
    message("Debug libraries used for acc")
    LIBS	+= \
        -L../../bil/debug -ldbd
} else {
    message("Release libraries used for acc")
    LIBS	+= \
        -L../../bil/release -ldb
}

# add forms, headers and sources
exists( ../src/acc.pri ) {
    include( ../src/acc.pri )
} else {
    # message and exit
    error( "An error has occurred, ../src/acc.pri is missing." )
}

#FORMS += \
#    gui/acc_mainwindow.ui

#HEADERS += \
# plugin
#    ../src/actions/acc_actionplugin.h \ part of .pri
# stand-alone application
#    gui/acc_mainwindow.h \
#    gui/main.h

#SOURCES += \
# plugin
#    ../src/actions/acc_actionplugin.cpp \ part of .pri
# stand-alone application
#    gui/acc_mainwindow.cpp \
#    gui/main.cpp

# install echoplugin example
#target.path = $$[QT_INSTALL_EXAMPLES]/tools/echoplugin
#sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS echoplugin.pro
#sources.path = $$[QT_INSTALL_EXAMPLES]/tools/echoplugin
#INSTALLS += target sources

#target.path = ../../bil/plugins
#INSTALLS += target
