#*****************************************************************
# $Id: defstest.pro 1447 2011-08-09 17:57:46Z rutger $
# Created: 2007-01-10
#
# Copyright (C) 2008 Red-Bag. All rights reserved.
# This file is part of the Biluna 'mkspecs' project.
#
# See http://www.red-bag.com for further details.
#*****************************************************************

# qmake project file to include in project files of biluna
# provides settings for release, debug, demo etc.

# webkitwidgets includes widgets
QT += core gui network printsupport sql svg testlib uitools webkitwidgets widgets xml
#CONFIG += release
CONFIG -= release
QT_DEBUG_PLUGINS = 1

if (release) {
    message("Release compile mode in defs.pro")
    CONFIG -= debug
    CONFIG -= debug_and_release
#    CONFIG += release
    CONFIG += rtti
#    CONFIG += testlib remove otherwise application starts with command line in MS Windows
#    CONFIG -= thread
    # OBJECTS_DIR = tmp/release/obj
    # MOC_DIR     = tmp/release/moc
    # RCC_DIR     = tmp/release/res
}
else {
    message("Debug compile mode in defs.pro")
    CONFIG -= release
    CONFIG -= debug_and_release
    CONFIG += debug
    CONFIG += rtti
    CONFIG += testlib
#	CONFIG -= thread
    # OBJECTS_DIR = tmp/debug/obj
    # MOC_DIR     = tmp/debug/moc
    # RCC_DIR     = tmp/debug/res
}

CONFIG += openssl-linked

UI_DIR	    = ../src/gui
UI_HEADERS_DIR = ../src/gui/ui

# win32-msvc {
#  DEFINES += _CRT_SECURE_NO_DEPRECATE
# }

#!win32-msvc {
#    SOURCES += /opt/qtsdk-2009.01/share/qtcreator/gdbmacros/gdbmacros.cpp
#}

INCLUDEPATH += ../mkspecs

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
    message(Debug $$TARGET will be created)
} else {
    message(Release $$TARGET will be created)
}

# OpenSSL
win32 {
    message("Using OpenSSL-Win32")
    INCLUDEPATH += \
        C:/OpenSSL-Win32/include \
        C:/OpenSSL-Win32/include/openssl

    # ssleay32.dll is old (is now libssl32.dll) but required for webkit
    LIBS += \
        -LC:/OpenSSL-Win32/lib -llibeay32 \
#        -LC:/OpenSSL-Win32/ -llibssl32 \
        -LC:/OpenSSL-Win32/lib -lssleay32

    #openSSL encryption and decryption
    HEADERS += \
        C:/OpenSSL-Win32/include/openssl/evp.h
}

macx {
    CONFIG -= app_bundle
    LIBS += -lssl -lcrypto
}

unix:!macx {
    INCLUDEPATH += /usr/include/openssl
    LIBS += /usr/bin/ -lopenssl
}

