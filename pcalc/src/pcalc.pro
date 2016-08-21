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
    # for qcad2, CAD_EXPORT and ui files created with cad.pro and cad.pri
    ../../cad/src/core \
    ../../cad/src/gui/ui \
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
    ../../thrd/qcad2/dxflib/src \
    ../../thrd/qcad2/fparser/src \
    ../../thrd/qcad2/qcadactions/src/blocks \
    ../../thrd/qcad2/qcadactions/src/debugging \
    ../../thrd/qcad2/qcadactions/src/dimension \
    ../../thrd/qcad2/qcadactions/src/draw \
    ../../thrd/qcad2/qcadactions/src/edit \
    ../../thrd/qcad2/qcadactions/src/file \
    ../../thrd/qcad2/qcadactions/src/info \
    ../../thrd/qcad2/qcadactions/src/layers \
    ../../thrd/qcad2/qcadactions/src/misc \
    ../../thrd/qcad2/qcadactions/src/modify \
    ../../thrd/qcad2/qcadactions/src/options \
    ../../thrd/qcad2/qcadactions/src/script \
    ../../thrd/qcad2/qcadactions/src/select \
    ../../thrd/qcad2/qcadactions/src/snap \
    ../../thrd/qcad2/qcadactions/src/view \
    ../../thrd/qcad2/qcadactions/src/zoom \
    ../../thrd/qcad2/qcadguiqt/src \
    ../../thrd/qcad2/qcadguiqt/src/dialogs \
    ../../thrd/qcad2/qcadguiqt/src/toolbars \
    ../../thrd/qcad2/qcadguiqt/src/tooloptions \
    ../../thrd/qcad2/qcadguiqt/src/widgets \
    ../../thrd/qcad2/qcadlib/src/actions \
    ../../thrd/qcad2/qcadlib/src/creation \
    ../../thrd/qcad2/qcadlib/src/entities \
    ../../thrd/qcad2/qcadlib/src/fileio\
    ../../thrd/qcad2/qcadlib/src/filters \
    ../../thrd/qcad2/qcadlib/src/global \
    ../../thrd/qcad2/qcadlib/src/gui \
    ../../thrd/qcad2/qcadlib/src/information \
    ../../thrd/qcad2/qcadlib/src/math \
    ../../thrd/qcad2/qcadlib/src/modification \
    ../../thrd/qcad2/qcadlib/src/scripting \
    ../../thrd/qcad2/qcadlib/src/tools \
    ../../thrd/qcad2/qcadprop \
    ../../thrd/qcad2/qcadsvg/src \
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
    message("Debug libraries used for pcalc")
    LIBS	+= \
        -L../../bil/debug -lcadd \
        -L../../bil/debug -ldbd
} else {
    message("Release libraries used for pcalc")
    LIBS	+= \
        -L../../bil/release -lcad \
        -L../../bil/release -ldb
}

# add forms, headers and sources
exists( ../src/pcalc.pri ) {
    include( ../src/pcalc.pri )
} else {
    # message and exit
    error( "An error has occurred, ../src/pcalc.pri is missing." )
}

#exists( ../../gv/src/gv.pri ) {
#    include( ../../gv/src/gv.pri )
#} else {
#    # message and exit
#    error( "An error has occurred, ../../gv/src/gv.pri is missing." )
#}

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


