TEMPLATE = lib
DESTDIR = lib
CONFIG += staticlib warn_on
CONFIG -= qt debug_and_release
CONFIG += release
CONFIG -= debug
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
HEADERS = src/fparser.h
SOURCES = src/fparser.cpp
TARGET = fparser
OBJECTS_DIR = .obj
MOC_DIR = .moc
