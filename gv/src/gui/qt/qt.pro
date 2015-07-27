include( ../../../shared.pri )

# detect Qt property browser solution:
#exists( $$(QTDIR)/../qtpropertybrowser-2.5-opensource/src/qtpropertybrowser.pri ):include( $$(QTDIR)/../qtpropertybrowser-2.5-opensource/src/qtpropertybrowser.pri )
#else:error("Qt Property Browser Module not found in $$(QTDIR)/../")
exists( ../../../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/qtpropertybrowser.pri ){
    INCLUDEPATH += ../../../../../qtsolutions/qtpropertybrowser-2.4-commercial/src \
        tmp/debug/moc
    SOURCES += \
        ../../../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/qtpropertybrowser.cpp \
        ../../../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/qtpropertymanager.cpp \
        ../../../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/qteditorfactory.cpp \
        ../../../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/qtvariantproperty.cpp \
        ../../../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/qttreepropertybrowser.cpp \
        ../../../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/qtbuttonpropertybrowser.cpp \
        ../../../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/qtgroupboxpropertybrowser.cpp \
        ../../../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/qtpropertybrowserutils.cpp
    HEADERS += \
        ../../../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/qtpropertybrowser.h \
        ../../../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/qtpropertymanager.h \
        ../../../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/qteditorfactory.h \
        ../../../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/qtvariantproperty.h \
        ../../../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/qttreepropertybrowser.h \
        ../../../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/qtbuttonpropertybrowser.h \
        ../../../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/qtgroupboxpropertybrowser.h \
        ../../../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/qtpropertybrowserutils_p.h
    RESOURCES += \
        ../../../../../qtsolutions/qtpropertybrowser-2.4-commercial/src/qtpropertybrowser.qrc

} else {
        error("Qt Property Browser Module not found in (qt.pro) $$(QTDIR)/../")
}


SOURCES += RFileSystemModel.cpp \
    RFontChooserWidget.cpp \
    RRulerQt.cpp \
    RLineweightEditorFactory.cpp \
    RLineweightPropertyManager.cpp \
    RLinetypePropertyManager.cpp \
    RLinetypeEditorFactory.cpp \
    RColorPropertyManager.cpp \
    RColorEditorFactory.cpp \
    RLinetypeCombo.cpp \
    RLineweightCombo.cpp \
    RColorCombo.cpp \
    RListWidget.cpp \
    RCommandLine.cpp \
    RMdiChildQt.cpp \
    RTextEdit.cpp \
    RMathLineEdit.cpp \
    RTreePropertyBrowser.cpp \
    RGraphicsSceneGl.cpp \
    RGraphicsViewGl.cpp \
    RGraphicsViewGl3d.cpp \
    RHelpBrowser.cpp \
    RGuiActionQt.cpp \
    RMainWindowQt.cpp \
    RGraphicsViewQt.cpp \
    RGraphicsSceneQt.cpp \
    RWebView.cpp \
    qgraphicsview/RGraphicsViewQV.cpp \
    qgraphicsview/RGraphicsSceneQV.cpp \
    qgraphicsview/RGraphicsLineItem.cpp \
    qgraphicsview/RGraphicsItem.cpp
HEADERS += RListView.h \
    RFileSystemModel.h \
    RFontChooserWidget.h \
    RRulerQt.h \
    RLineweightEditorFactory.h \
    RLineweightPropertyManager.h \
    RLinetypePropertyManager.h \
    RLinetypeEditorFactory.h \
    RAbstractEditorFactory.h \
    RColorPropertyManager.h \
    RColorEditorFactory.h \
    RLinetypeCombo.h \
    RLineweightCombo.h \
    RColorCombo.h \
    RListWidget.h \
    RCommandLine.h \
    RMdiChildQt.h \
    RTextEdit.h \
    RMathLineEdit.h \
    RTreePropertyBrowser.h \
    RGraphicsSceneGl.h \
    RGraphicsViewGl.h \
    RGraphicsViewGl3d.h \
    RHelpBrowser.h \
    RGuiActionQt.h \
    RMainWindowQt.h \
    RGraphicsViewQt.h \
    RGraphicsSceneQt.h \
    RWebView.h \
    qgraphicsview/RGraphicsViewQV.h \
    qgraphicsview/RGraphicsSceneQV.h \
    qgraphicsview/RGraphicsLineItem.h \
    qgraphicsview/RGraphicsItem.h
TEMPLATE = lib
CONFIG += staticlib
TARGET = guiqt
