TEMPLATE = lib
DESTDIR = lib
CONFIG += staticlib warn_on
CONFIG -= qt debug_and_release
CONFIG += release
CONFIG -= debug

win32-msvc {
  DEFINES += _CRT_SECURE_NO_DEPRECATE
}

macx {
  exists(/Developer/SDKs/MacOSX10.5.sdk) {
    QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.5.sdk
    CONFIG+=x86 ppc
  }
  else {
    exists(/Developer/SDKs/MacOSX10.4u.sdk) {
      QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
      CONFIG+=x86 ppc
    }
  }
  QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.3
}

INCLUDEPATH += src
HEADERS = \
    src/dl_attributes.h\
    src/dl_codes.h\
    src/dl_creationadapter.h\
    src/dl_creationinterface.h\
    src/dl_dxf.h\
    src/dl_entities.h\
    src/dl_exception.h\
    src/dl_extrusion.h\
    src/dl_writer.h\
    src/dl_writer_ascii.h
SOURCES = \
    src/dl_dxf.cpp\
    src/dl_writer_ascii.cpp
TARGET = libdxf
OBJECTS_DIR = .obj
