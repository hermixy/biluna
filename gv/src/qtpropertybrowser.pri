exists( ../../thrd/qt-solutions/qtpropertybrowser/src/qtpropertybrowser.pri ){
    INCLUDEPATH += ../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/
    SOURCES += \
        ../../thrd/qt-solutions/qtpropertybrowser/src/qtpropertybrowser.cpp \
        ../../thrd/qt-solutions/qtpropertybrowser/src/qtpropertymanager.cpp \
        ../../thrd/qt-solutions/qtpropertybrowser/src/qteditorfactory.cpp \
        ../../thrd/qt-solutions/qtpropertybrowser/src/qtvariantproperty.cpp \
        ../../thrd/qt-solutions/qtpropertybrowser/src/qttreepropertybrowser.cpp \
        ../../thrd/qt-solutions/qtpropertybrowser/src/qtbuttonpropertybrowser.cpp \
        ../../thrd/qt-solutions/qtpropertybrowser/src/qtgroupboxpropertybrowser.cpp \
        ../../thrd/qt-solutions/qtpropertybrowser/src/qtpropertybrowserutils.cpp
    HEADERS += \
        ../../thrd/qt-solutions/qtpropertybrowser/src/qtpropertybrowser.h \
        ../../thrd/qt-solutions/qtpropertybrowser/src/qtpropertymanager.h \
        ../../thrd/qt-solutions/qtpropertybrowser/src/qteditorfactory.h \
        ../../thrd/qt-solutions/qtpropertybrowser/src/qtvariantproperty.h \
        ../../thrd/qt-solutions/qtpropertybrowser/src/qttreepropertybrowser.h \
        ../../thrd/qt-solutions/qtpropertybrowser/src/qtbuttonpropertybrowser.h \
        ../../thrd/qt-solutions/qtpropertybrowser/src/qtgroupboxpropertybrowser.h \
        ../../thrd/qt-solutions/qtpropertybrowser/src/qtpropertybrowserutils_p.h
    RESOURCES += \
        ../../thrd/qt-solutions/qtpropertybrowser/src/qtpropertybrowser.qrc

} else {
        error("Qt Property Browser Module not found in (1) ../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/")
}
