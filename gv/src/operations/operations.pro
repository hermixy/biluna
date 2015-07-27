include( ../../shared.pri )
SOURCES += RAddObjectOperation.cpp \
    RAddObjectsOperation.cpp \
    RChangePropertyOperation.cpp \
    RClipboardOperation.cpp \
    RCopyOperation.cpp \
    RDeleteAllEntitiesOperation.cpp \
    RDeleteObjectOperation.cpp \
    RDeleteObjectsOperation.cpp \
    RDeleteSelectionOperation.cpp \
    RMoveReferencePointOperation.cpp \
    RMoveSelectionOperation.cpp \
    RPasteOperation.cpp
HEADERS = RAddObjectOperation.h \
    RAddObjectsOperation.h \
    RChangePropertyOperation.h \
    RClipboardOperation.h \
    RCopyOperation.h \
    RDeleteAllEntitiesOperation.h \
    RDeleteObjectOperation.h \
    RDeleteObjectsOperation.h \
    RDeleteSelectionOperation.h \
    RModifyObjectOperation.h \
    RModifyObjectsOperation.h \
    RMoveReferencePointOperation.h \
    RMoveSelectionOperation.h \
    RPasteOperation.h
TEMPLATE = lib
CONFIG += staticlib
