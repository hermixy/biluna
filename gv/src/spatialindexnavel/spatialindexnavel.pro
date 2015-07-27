include( ../../shared.pri )

TEMPLATE = lib
CONFIG += staticlib

# this is required to compile the spatial index library from navel ltd:
DEFINES += PACKAGE_BUGREPORT=\\\"mhadji@gmail.com\\\"

# suppress spatial debug messages
DEFINES += NDEBUG

CONFIG -= warn_on
CONFIG += warn_off

HEADERS = \
    RSpatialIndexNavel.h \
    spatialindex/include/tools/PointerPool.h \
    spatialindex/include/tools/PoolPointer.h \
    spatialindex/include/tools/SmartPointer.h \
    spatialindex/include/tools/Tools.h \
    spatialindex/include/tools/rand48.h \
    spatialindex/include/Version.h \
    spatialindex/include/MovingPoint.h \
    spatialindex/include/Point.h \
    spatialindex/include/LineSegment.h \
    spatialindex/include/RTree.h \
    spatialindex/include/TimeRegion.h \
    spatialindex/include/MovingRegion.h \
    spatialindex/include/Region.h \
    spatialindex/include/MVRTree.h \
    spatialindex/include/SpatialIndex.h \
    spatialindex/include/TPRTree.h \
    spatialindex/include/TimePoint.h \
    spatialindex/src/storagemanager/Buffer.h \
    spatialindex/src/storagemanager/DiskStorageManager.h \
    spatialindex/src/storagemanager/RandomEvictionsBuffer.h \
    spatialindex/src/storagemanager/MemoryStorageManager.h \
    spatialindex/src/rtree/RIndex.h \
    spatialindex/src/rtree/RLeaf.h \
    spatialindex/src/rtree/RNode.h \
    spatialindex/src/rtree/RStatistics.h \
    spatialindex/src/rtree/RTree.h \
    spatialindex/src/rtree/RBulkLoader.h \
    spatialindex/src/rtree/RPointerPoolNode.h \
    spatialindex/src/mvrtree/MVRStatistics.h \
    spatialindex/src/mvrtree/MVRLeaf.h \
    spatialindex/src/mvrtree/MVRTree.h \
    spatialindex/src/mvrtree/MVRIndex.h \
    spatialindex/src/mvrtree/MVRNode.h \
    spatialindex/src/mvrtree/MVRPointerPoolNode.h \
    spatialindex/src/spatialindex/SpatialIndexImpl.h \
    spatialindex/src/tprtree/TPRTree.h \
    spatialindex/src/tprtree/TPRStatistics.h \
    spatialindex/src/tprtree/TPRIndex.h \
    spatialindex/src/tprtree/TPRLeaf.h \
    spatialindex/src/tprtree/TPRNode.h \
    spatialindex/src/tprtree/TPRPointerPoolNode.h

SOURCES = \
    RSpatialIndexNavel.cpp \
    spatialindex/src/storagemanager/RandomEvictionsBuffer.cc \
    spatialindex/src/storagemanager/MemoryStorageManager.cc \
    spatialindex/src/storagemanager/Buffer.cc \
    spatialindex/src/storagemanager/DiskStorageManager.cc \
    spatialindex/src/tools/rand48.cc \
    spatialindex/src/tools/Tools.cc \
    spatialindex/src/rtree/RIndex.cc \
    spatialindex/src/rtree/RLeaf.cc \
    spatialindex/src/rtree/RNode.cc \
    spatialindex/src/rtree/RTree.cc \
    spatialindex/src/rtree/RBulkLoader.cc \
    spatialindex/src/rtree/RStatistics.cc \
    spatialindex/src/mvrtree/MVRStatistics.cc \
    spatialindex/src/mvrtree/MVRIndex.cc \
    spatialindex/src/mvrtree/MVRTree.cc \
    spatialindex/src/mvrtree/MVRLeaf.cc \
    spatialindex/src/mvrtree/MVRNode.cc \
    spatialindex/src/spatialindex/TimeRegion.cc \
    spatialindex/src/spatialindex/MovingRegion.cc \
    spatialindex/src/spatialindex/Region.cc \
    spatialindex/src/spatialindex/SpatialIndexImpl.cc \
    spatialindex/src/spatialindex/TimePoint.cc \
    spatialindex/src/spatialindex/MovingPoint.cc \
    spatialindex/src/spatialindex/Point.cc \
    spatialindex/src/spatialindex/LineSegment.cc \
    spatialindex/src/tprtree/TPRTree.cc \
    spatialindex/src/tprtree/TPRIndex.cc \
    spatialindex/src/tprtree/TPRNode.cc \
    spatialindex/src/tprtree/TPRLeaf.cc \
    spatialindex/src/tprtree/TPRStatistics.cc
