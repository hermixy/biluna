#-------------------------------------------------
# $Id: acc.pro 1352 2011-01-26 22:01:25Z rutger $
# Project created 2011-02-15
# - for building Plant Engineering (2D) PENG plugin
#-------------------------------------------------

TEMPLATE    = lib
LANGUAGE    = C++
TARGET      = peng_actionplugin

CONFIG      += qt plugin warn_on
QT          += core network sql svg webkit xml

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
    ../../db/src/filters \
    ../../db/src/gui \
    ../../db/src/gui/ui \
    ../../db/src/model \
    ../../db/src/operations \
    ../../db/src/utils \
    ../../db/src/visitors \
    ../../gl/src/model \
    ../../gv/src/actions \
    ../../gv/src/core \
    ../../gv/src/filters \
    ../../gv/src/gui \
    ../../gv/src/gui/ui \
    ../../gv/src/model \
    ../../gv/src/math \
    ../../gv/src/operations \
#    ../../rpt/src/actions \
#    ../../rpt/src/model \
#    ../../rpt/src/filters \
#    ../../rpt/src/gui \
#    ../../rpt/src/gui/ui \
#    ../../rpt/src/operations \
    ../src/actions \
    ../src/filters \
    ../src/gui \
    ../src/gui/ui \
    ../src/math \
    ../src/model \
    ../src/model/EN1591 \
    ../src/operations \
    ../src/utils

# Carefull with the sequence of the libraries, most basic one last
CONFIG(debug, debug|release) {
    message("Debug libraries used for peng")
    LIBS	+= \
#        -L../../rpt/lib -lrptd \
#        -L../../bil/debug -lgvd \
        -L../../bil/debug -ldbd
} else {
    message("Release libraries used for peng")
    LIBS	+= \
#        -L../../rpt/lib -lrpt \
#        -L../../bil/release -lgv \
        -L../../bil/release -ldb
}

# add forms, headers and sources
exists( ../src/peng.pri ) {
    include( ../src/peng.pri )
} else {
    # message and exit
    error( "An error has occurred, ../src/peng.pri is missing." )
}

#FORMS += \

#HEADERS += \
#    ../src/actions/peng_actionplugin.h \

#SOURCES += \
#    ../src/actions/acc_actionplugin.cpp \

# install echoplugin example
#target.path = $$[QT_INSTALL_EXAMPLES]/tools/echoplugin
#sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS echoplugin.pro
#sources.path = $$[QT_INSTALL_EXAMPLES]/tools/echoplugin
#INSTALLS += target sources

#target.path = ../../bil/plugins
#INSTALLS += target
