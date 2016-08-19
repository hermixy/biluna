TEMPLATE    = lib
DESTDIR     = lib
CONFIG      += qt staticlib warn_on windows

exists( ../mkspecs/defs.pro ) {
    include( ../mkspecs/defs.pro )
}

# updated path to propertybrowser by rutger
!win32 {
    exists(../../workspace/thrd/qt-solutions/qtpropertybrowser/qtpropertybrowser.pri) {
        include(../../workspace/thrd/qt-solutions/qtpropertybrowser/qtpropertybrowser.pri)
    }
    else {
        message("Qt Solution \"Qt Property Browser\" sources not found")
    }
}

win32 {
    include(../../workspace/thrd/qt-solutions/qtpropertybrowser/qtpropertybrowser.pri)
}

TARGET      = qcadprop

OBJECTS_DIR = .obj
MOC_DIR     = .moc
RCC_DIR     = .res

