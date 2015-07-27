#VERSION = 3.0.0

# detect Qt property browser solution:
#exists( ../qtsolutions/qtpropertybrowser-2.4-commercial/src/qtpropertybrowser.pri ){
#    INCLUDEPATH += ../qtsolutions/qtpropertybrowser-2.4-commercial/src
#} else {
#    error("Qt Property Browser Module not found in (2) $$(QTDIR)/../")
#}

# detect OpenDesign libraries:
macx:exists( $$PWD/debug/opendesign/DD_Db.a ) {
	message("OpenDesign libraries for Mac OS X found.")
} else:unix:exists( $$PWD/debug/opendesign/DD_Db.a ) {
	message("OpenDesign libraries for Linux found.")
} else {
    warning("OpenDesign libraries not found. Building without DWG support.")
    CONFIG += rnodwg
    DEFINES += RNODWG
}

QT += core \
    script \
    scripttools \
    sql \
    opengl \
    webkit \
    network \
    xmlpatterns

# for spatial index lib:
DEFINES += PTHREADS
DEFINES += HAVE_PTHREAD_H

CONFIG -= warn_off
CONFIG += warn_on
#CONFIG -= warn_on
#CONFIG += warn_off
CONFIG += debug_and_release ordered embed_manifest_dll
#CONFIG += x86 x86_64
QMAKE_CXXFLAGS += -Wno-unused-parameter

#QMAKE_TARGET_COMPANY = RibbonSoft GmbH
#QMAKE_TARGET_DESCRIPTION = QCAD Framework
#QMAKE_TARGET_COPYRIGHT = 2009 RibbonSoft GmbH
#QMAKE_TARGET_PRODUCT = RS Framework

# ccache win32 port: http://code.google.com/p/ccache-win32/
win32-g++ {
    exists(C:\MinGW\bin\ccache.exe) {
        QMAKE_CXX = C:\MinGW\bin\ccache.exe g++
    }
}

!win32 {
    CCACHE = $$system(ccache -V)
    contains(CCACHE, Andrew) {
        QMAKE_CXX = ccache g++
        QMAKE_CC = ccache gcc
    }
}

#win32-msvc* {
#	message("win32-msvc*")
#	QMAKE_CXXFLAGS += /FIstdint.h
#}

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/debug
    LIBS += -L$$PWD/debug \
            -L$$PWD/debug/opendesign
    #QMAKE_LFLAGS += -rdynamic
    #QMAKE_CXXFLAGS_DEBUG -= -g
    #QMAKE_CXXFLAGS_DEBUG += -g3
    #QMAKE_CXXFLAGS_DEBUG += -pg
    exists(/usr/include/bfd.h) {
        CONFIG += rbacktrace
        DEFINES += RBACKTRACE
    }
} else {
    DESTDIR = $$PWD/release
    LIBS += -L$$PWD/release \
            -L$$PWD/release/opendesign
}

INCLUDEPATH += \
$$PWD/src/core \
$$PWD/src/grid \
$$PWD/src/snap \
$$PWD/src/gui/qt \
$$PWD/src/gui/qt/qgraphicsview \
$$PWD/src/entity \
$$PWD/src/math \
$$PWD/src/operations \
$$PWD/src/storage/db \
$$PWD/src/spatialindexnavel \
$$PWD/src/xml/xml \
$$PWD/src/xml/xslt \
$$PWD/src/scripting \
$$PWD/src/scripting/ecma \
$$PWD/src/scripting/ecmaapi \
$$PWD/src/plugins/qt \
$$PWD/src/io/dwg/opendesign/Include \
$$PWD/src/io/dwg/opendesign/Extensions/ExServices \
$$PWD/src/io/dwg \
$$PWD/src/xml/xslt/libxslt \
$$PWD/src/xml/xml/libxml2/include
