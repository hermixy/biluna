#-------------------------------------------------
# $Id: acc.pro 1352 2011-01-26 22:01:25Z rutger $
# Project created 2015-04-28
# - for building Plant Calculation PCALC plugin
#-------------------------------------------------

TEMPLATE    = lib
LANGUAGE    = C++
TARGET      = pcalc_actionplugin

CONFIG      += plugin
QT          += charts

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
    ../../gv/src/entity \
    ../../gv/src/gui \
    ../../gv/src/model \
    ../../gv/src/tools \
    ../../peng/src/gui \
    ../../peng/src/gui/ui \
    ../../peng/src/model \
    ../src/actions \
    ../src/calc \
    ../src/calc/EN1591 \
    ../src/calc/EN1591/unittest \
    ../src/calc/unittest \
    ../src/filters \
    ../src/gui \
    ../src/gui/ui \
    ../src/math \
    ../src/model \
    ../src/model/EN1591 \
    ../src/operations \
    ../src/utils \
    ../src/utils/EN1591 \
    ../src/utils/unittest

# Carefull with the sequence of the libraries, most basic one last
CONFIG(debug, debug|release) {
    message("Debug libraries used for peng")
    LIBS	+= \
        -L../../bil/debug -ldbd
} else {
    message("Release libraries used for peng")
    LIBS	+= \
        -L../../bil/release -ldb
}

# add forms, headers and sources
exists( ../src/pcalc.pri ) {
    include( ../src/pcalc.pri )
} else {
    # message and exit
    error( "An error has occurred, ../src/pcalc.pri is missing." )
}

exists( ../../gv/src/gv.pri ) {
    include( ../../gv/src/gv.pri )
} else {
    # message and exit
    error( "An error has occurred, ../../gv/src/gv.pri is missing." )
}

#FORMS += \

#HEADERS += \

#SOURCES += \

# install echoplugin example
#target.path = $$[QT_INSTALL_EXAMPLES]/tools/echoplugin
#sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS echoplugin.pro
#sources.path = $$[QT_INSTALL_EXAMPLES]/tools/echoplugin
#INSTALLS += target sources

#target.path = ../../bil/plugins
#INSTALLS += target


