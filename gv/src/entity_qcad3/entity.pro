include( ../../shared.pri )
SOURCES = RArcData.cpp \
    RArcEntity.cpp \
    RCircleData.cpp \
    RCircleEntity.cpp \
    RPointData.cpp \
    RPointEntity.cpp \
    RLineEntity.cpp \
    RLineData.cpp
HEADERS = RArcData.h \
    RArcEntity.h \
    RCircleData.h \
    RCircleEntity.h \
    RPointData.h \
    RPointEntity.h \
    RLineEntity.h \
    RLineData.h
TEMPLATE = lib
CONFIG += staticlib
