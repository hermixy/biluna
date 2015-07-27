#include( shared.pri )

#TEMPLATE = lib
#CONFIG += staticlib

# for spatial index lib:
DEFINES += PTHREADS
DEFINES += HAVE_PTHREAD_H
CONFIG += ordered embed_manifest_dll
#QMAKE_CXXFLAGS += -Wno-unused-parameter

# this is required to compile the spatial index library from navel ltd:
DEFINES += PACKAGE_BUGREPORT=\\\"mhadji@gmail.com\\\"

# suppress spatial debug messages
DEFINES += NDEBUG

#CONFIG -= warn_on
#CONFIG += warn_off

HEADERS += \
    ../src/spatialindexnavel/RSpatialIndexNavel.h \
    ../src/spatialindexnavel/spatialindex/include/tools/PointerPool.h \
    ../src/spatialindexnavel/spatialindex/include/tools/PoolPointer.h \
    ../src/spatialindexnavel/spatialindex/include/tools/SmartPointer.h \
    ../src/spatialindexnavel/spatialindex/include/tools/Tools.h \
    ../src/spatialindexnavel/spatialindex/include/tools/rand48.h \
    ../src/spatialindexnavel/spatialindex/include/Version.h \
    ../src/spatialindexnavel/spatialindex/include/MovingPoint.h \
    ../src/spatialindexnavel/spatialindex/include/Point.h \
    ../src/spatialindexnavel/spatialindex/include/LineSegment.h \
    ../src/spatialindexnavel/spatialindex/include/RTree.h \
    ../src/spatialindexnavel/spatialindex/include/TimeRegion.h \
    ../src/spatialindexnavel/spatialindex/include/MovingRegion.h \
    ../src/spatialindexnavel/spatialindex/include/Region.h \
    ../src/spatialindexnavel/spatialindex/include/MVRTree.h \
    ../src/spatialindexnavel/spatialindex/include/SpatialIndex.h \
    ../src/spatialindexnavel/spatialindex/include/TPRTree.h \
    ../src/spatialindexnavel/spatialindex/include/TimePoint.h \
    ../src/spatialindexnavel/spatialindex/src/storagemanager/Buffer.h \
    ../src/spatialindexnavel/spatialindex/src/storagemanager/DiskStorageManager.h \
    ../src/spatialindexnavel/spatialindex/src/storagemanager/RandomEvictionsBuffer.h \
    ../src/spatialindexnavel/spatialindex/src/storagemanager/MemoryStorageManager.h \
    ../src/spatialindexnavel/spatialindex/src/rtree/RIndex.h \
    ../src/spatialindexnavel/spatialindex/src/rtree/RLeaf.h \
    ../src/spatialindexnavel/spatialindex/src/rtree/RNode.h \
    ../src/spatialindexnavel/spatialindex/src/rtree/RStatistics.h \
    ../src/spatialindexnavel/spatialindex/src/rtree/RTree.h \
    ../src/spatialindexnavel/spatialindex/src/rtree/RBulkLoader.h \
    ../src/spatialindexnavel/spatialindex/src/rtree/RPointerPoolNode.h \
    ../src/spatialindexnavel/spatialindex/src/mvrtree/MVRStatistics.h \
    ../src/spatialindexnavel/spatialindex/src/mvrtree/MVRLeaf.h \
    ../src/spatialindexnavel/spatialindex/src/mvrtree/MVRTree.h \
    ../src/spatialindexnavel/spatialindex/src/mvrtree/MVRIndex.h \
    ../src/spatialindexnavel/spatialindex/src/mvrtree/MVRNode.h \
    ../src/spatialindexnavel/spatialindex/src/mvrtree/MVRPointerPoolNode.h \
    ../src/spatialindexnavel/spatialindex/src/spatialindex/SpatialIndexImpl.h \
    ../src/spatialindexnavel/spatialindex/src/tprtree/TPRTree.h \
    ../src/spatialindexnavel/spatialindex/src/tprtree/TPRStatistics.h \
    ../src/spatialindexnavel/spatialindex/src/tprtree/TPRIndex.h \
    ../src/spatialindexnavel/spatialindex/src/tprtree/TPRLeaf.h \
    ../src/spatialindexnavel/spatialindex/src/tprtree/TPRNode.h \
    ../src/spatialindexnavel/spatialindex/src/tprtree/TPRPointerPoolNode.h

SOURCES += \
    ../src/spatialindexnavel/RSpatialIndexNavel.cpp \
    ../src/spatialindexnavel/spatialindex/src/storagemanager/RandomEvictionsBuffer.cc \
    ../src/spatialindexnavel/spatialindex/src/storagemanager/MemoryStorageManager.cc \
    ../src/spatialindexnavel/spatialindex/src/storagemanager/Buffer.cc \
    ../src/spatialindexnavel/spatialindex/src/storagemanager/DiskStorageManager.cc \
    ../src/spatialindexnavel/spatialindex/src/tools/rand48.cc \
    ../src/spatialindexnavel/spatialindex/src/tools/Tools.cc \
    ../src/spatialindexnavel/spatialindex/src/rtree/RIndex.cc \
    ../src/spatialindexnavel/spatialindex/src/rtree/RLeaf.cc \
    ../src/spatialindexnavel/spatialindex/src/rtree/RNode.cc \
    ../src/spatialindexnavel/spatialindex/src/rtree/RTree.cc \
    ../src/spatialindexnavel/spatialindex/src/rtree/RBulkLoader.cc \
    ../src/spatialindexnavel/spatialindex/src/rtree/RStatistics.cc \
    ../src/spatialindexnavel/spatialindex/src/mvrtree/MVRStatistics.cc \
    ../src/spatialindexnavel/spatialindex/src/mvrtree/MVRIndex.cc \
    ../src/spatialindexnavel/spatialindex/src/mvrtree/MVRTree.cc \
    ../src/spatialindexnavel/spatialindex/src/mvrtree/MVRLeaf.cc \
    ../src/spatialindexnavel/spatialindex/src/mvrtree/MVRNode.cc \
    ../src/spatialindexnavel/spatialindex/src/spatialindex/TimeRegion.cc \
    ../src/spatialindexnavel/spatialindex/src/spatialindex/MovingRegion.cc \
    ../src/spatialindexnavel/spatialindex/src/spatialindex/Region.cc \
    ../src/spatialindexnavel/spatialindex/src/spatialindex/SpatialIndexImpl.cc \
    ../src/spatialindexnavel/spatialindex/src/spatialindex/TimePoint.cc \
    ../src/spatialindexnavel/spatialindex/src/spatialindex/MovingPoint.cc \
    ../src/spatialindexnavel/spatialindex/src/spatialindex/Point.cc \
    ../src/spatialindexnavel/spatialindex/src/spatialindex/LineSegment.cc \
    ../src/spatialindexnavel/spatialindex/src/tprtree/TPRTree.cc \
    ../src/spatialindexnavel/spatialindex/src/tprtree/TPRIndex.cc \
    ../src/spatialindexnavel/spatialindex/src/tprtree/TPRNode.cc \
    ../src/spatialindexnavel/spatialindex/src/tprtree/TPRLeaf.cc \
    ../src/spatialindexnavel/spatialindex/src/tprtree/TPRStatistics.cc
