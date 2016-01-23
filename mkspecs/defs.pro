#*****************************************************************
# $Id: defs.pro 2251 2015-06-22 11:56:22Z rutger $
# Created: 2007-01-10
#
# Copyright (C) 2008 Red-Bag. All rights reserved.
# This file is part of the Biluna 'mkspecs' project.
#
# See http://www.red-bag.com for further details.
#*****************************************************************

# qmake project file to include in project files of biluna
# provides settings for release, debug, demo etc.

# UI_DIR overrides both UI_HEADERS_DIR and UI_SOURCES_DIR
UI_DIR	    = ../src/gui/ui
INCLUDEPATH += ../mkspecs

QT += core gui help network printsupport sql svg uitools webenginewidgets widgets xml

#CONFIG += release set in qmake setting of QtCreator
CONFIG += c++11
CONFIG += qt warn_on rtti
CONFIG += embed_manifest_exe

CONFIG(debug, debug|release) {
#    message("Debug compile mode in defs.pro")
    # QT += testlib
    CONFIG -= release
    CONFIG -= debug_and_release
    CONFIG += debug
    # CONFIG -= thread
    OBJECTS_DIR = ../build_debug/obj
    MOC_DIR     = ../build_debug/moc
    RCC_DIR     = ../build_debug/res

    DEFINES += BILUNA_DEBUG

    QT_DEBUG_PLUGINS=1
}
else {
#    message("Release compile mode in defs.pro")
    QT -= testlib #remove otherwise MS Windows starts with command line
    CONFIG -= debug
    CONFIG -= debug_and_release
    # CONFIG += release
    # CONFIG -= thread
    OBJECTS_DIR = ../build_release/obj
    MOC_DIR     = ../build_release/moc
    RCC_DIR     = ../build_release/res

    if (demo) {
        DEFINES += BILUNA_DEMO
        DEFINES += QT_NO_PRINTER
    }
}

# Target path will be overriden in bil.pro and test applications
CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
    message(Debug $$TARGET will be created)
    CONFIG(plugin) {
        # plugin
        DESTDIR = ../lib
        target.path = ../../bil/debug/plugins
        INSTALLS += target
    }
    else {
        CONFIG(RB_DYNLIB) {
            # static or dynamic lib
            DESTDIR = ../lib
            target.path = ../../bil/debug
            INSTALLS += target
        }
        else {
            # executable
            DESTDIR = ../../bil/debug
        }
    }
}
else {
    message(Release $$TARGET will be created)
    CONFIG(plugin) {
                # plugin
        DESTDIR = ../lib
        target.path = ../../bil/release/plugins
        INSTALLS += target
    }
    else {
        CONFIG(RB_DYNLIB) {
            # static or dynamic lib
            # TODO: openssl-linked should be for libdb only
            message(Release openssl-linked)
            CONFIG += openssl-linked
            DESTDIR = ../lib
            target.path = ../../bil/release
            INSTALLS += target
        }
        else {
            # executable
            DESTDIR = ../../bil/release
        }
    }
}

# Check for host architecture
#message($$QT_CONFIG)
#message($$QMAKE_TARGET.arch)

# OpenSSL
win32 {
!contains(QMAKE_TARGET.arch, x86_64) {
    message("Using OpenSSL-Win32")
    INCLUDEPATH += \
        C:/OpenSSL-Win32/include \
        C:/OpenSSL-Win32/include/openssl

    # ssleay32.dll is old (is now libssl32.dll) but required for webkit engine?
    LIBS += \
        -LC:/OpenSSL-Win32/lib -llibeay32 \
#        -LC:/OpenSSL-Win32/ -llibssl32 \
        -LC:/OpenSSL-Win32/lib -lssleay32

    #openSSL encryption and decryption
    HEADERS += \
        C:/OpenSSL-Win32/include/openssl/evp.h
}
else {
    message("Using OpenSSL-Win64")
    INCLUDEPATH += \
        C:/OpenSSL-Win64/include \
        C:/OpenSSL-Win64/include/openssl

    # ssleay32.dll is old (is now libssl32.dll)?
    LIBS += \
        -LC:/OpenSSL-Win64/lib -llibeay32 \
#        -LC:/OpenSSL-Win64/ -llibssl32 \
        -LC:/OpenSSL-Win64/lib -lssleay32

    #openSSL encryption and decryption
    HEADERS += \
        C:/OpenSSL-Win64/include/openssl/evp.h
}
}

macx {
    CONFIG -= app_bundle
    #openssl
    INCLUDEPATH += \
        /usr/local/ssl/include
    LIBS += \
        -L/usr/local/ssl/lib -lssl \
        -L/usr/local/ssl/lib -lcrypto
#    HEADERS += \
#        /Users/gudula/Documents/biluna/thrd/openssl-1.0.2d/include/openssl/evp.h
    #latest architecture
    QMAKE_MAC_SDK = macosx10.11
}

unix:!macx {
    INCLUDEPATH += /usr/include/openssl
    LIBS += /usr/bin/ -lopenssl
    OPENSSL_LIBS= \
        -L/opt/ssl/lib -lssl -lcrypto
}

