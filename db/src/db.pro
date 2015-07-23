# ----------------------------------------------------------
# $Id: db.pro 2248 2015-06-21 09:13:00Z rutger $
# src.pro is renamed db.pro for Windows only
# - for building db lib
# ----------------------------------------------------------

# Type of binary, name and destination
TEMPLATE = lib
LANGUAGE = C++
TARGET   = db

CONFIG += openssl-linked
CONFIG += shared

# Biluna specific
CONFIG += RB_DYNLIB
DEFINES += DB_DLL

# Compiler settings for release, debug and path for ui, moc, binary files
exists( ../../mkspecs/defs.pro ) {
    include( ../../mkspecs/defs.pro )
}
else {
    # message and exit
    error( "An error has occurred, ../../mkspecs/defs.pro is missing." )
}

INCLUDEPATH += \
    actions \
    entities \
    filters \
    gui \
    gui/ui \
    model \
    operations \
    utils \
    visitors \
    ../../thrd/browser \
    ../../thrd/browser/data \
    ../../thrd/browser/htmls \
    ../../thrd/nightcharts \
    ../../thrd/qhexedit2/src \
    ../../thrd/speedcrunch-0.10.1/src \
    ../../thrd/speedcrunch-0.10.1/src/3rdparty \
    ../../thrd/speedcrunch-0.10.1/src/3rdparty/util \
    ../../thrd/speedcrunch-0.10.1/src/base \
#    ../../thrd/speedcrunch-0.10.1/src/books \
    ../../thrd/speedcrunch-0.10.1/src/gui \
#    ../../thrd/speedcrunch-0.10.1/src/i18n \
#    ../../thrd/speedcrunch-0.10.1/src/main \
    ../../thrd/speedcrunch-0.10.1/src/math \
    ../../thrd/speedcrunch-0.10.1/src/resources
#    ../../thrd/speedcrunch-0.10.1/src/test \

# LIBS		+=  -lmysqlclient -L/usr/lib/mysql/

# Add forms, headers and sources
exists( db.pri) {
    include( db.pri )
} else {
    # message and exit
    error( "An error has occurred, db.pri is missing." )
}

# In case of release remove header and source debug files from .pri
CONFIG (release) {
    HEADERS -= \
        ../src/utils/rb_signalspydialog.h
    SOURCES -= \
        ../src/utils/rb_signalspydialog.cpp
}

# Browser, Nightcharts, Speedcrunch calculator
exists( thrd.pri ) {
    include( thrd.pri )
} else {
    error( "An error has occurred, thrd.pri is missing." )
}

# install echoplugin example
#target.path = $$[QT_INSTALL_EXAMPLES]/tools/echoplugin
#sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS echoplugin.pro
#sources.path = $$[QT_INSTALL_EXAMPLES]/tools/echoplugin
#INSTALLS += target sources

#target.path = ../../bil
#INSTALLS += target
