include( ../../shared.pri )
SOURCES = \
    RSnapAuto.cpp \
    RSnapCenter.cpp \
    RSnapDistance.cpp \
    RSnapEnd.cpp \
    RSnapEntityBase.cpp \
    RSnapFree.cpp \
    RSnapGrid.cpp \
    RSnapIntersection.cpp \
    RSnapMiddle.cpp \
    RSnapOnEntity.cpp \
    RSnapReference.cpp
HEADERS = \
    RSnapAuto.h \
    RSnapCenter.h \
    RSnapDistance.h \
    RSnapEnd.h \
    RSnapEntityBase.h \
    RSnapFree.h \
    RSnapGrid.h \
    RSnapIntersection.h \
    RSnapMiddle.h \
    RSnapOnEntity.h \
    RSnapReference.h 
TEMPLATE = lib
CONFIG += staticlib
