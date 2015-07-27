#include( ../../../shared.pri )

# detect Qt property browser solution:
#exists( $$(QTDIR)/../qtpropertybrowser/qtpropertybrowser.pri ):include( $$(QTDIR)/../qtpropertybrowser/qtpropertybrowser.pri )
#else:error("Qt Property Browser Module not found in $$(QTDIR)/../")


SOURCES += \
    ../../gv/src/gui/qt/RFileSystemModel.cpp \
    ../../gv/src/gui/qt/RFontChooserWidget.cpp \
    ../../gv/src/gui/qt/RRulerQt.cpp \
    ../../gv/src/gui/qt/RLineweightEditorFactory.cpp \
    ../../gv/src/gui/qt/RLineweightPropertyManager.cpp \
    ../../gv/src/gui/qt/RLinetypePropertyManager.cpp \
    ../../gv/src/gui/qt/RLinetypeEditorFactory.cpp \
    ../../gv/src/gui/qt/RColorPropertyManager.cpp \
    ../../gv/src/gui/qt/RColorEditorFactory.cpp \
    ../../gv/src/gui/qt/RLinetypeCombo.cpp \
    ../../gv/src/gui/qt/RLineweightCombo.cpp \
    ../../gv/src/gui/qt/RColorCombo.cpp \
    ../../gv/src/gui/qt/RListWidget.cpp \
    ../../gv/src/gui/qt/RCommandLine.cpp \
    ../../gv/src/gui/qt/RMdiChildQt.cpp \
    ../../gv/src/gui/qt/RTextEdit.cpp \
    ../../gv/src/gui/qt/RMathLineEdit.cpp \
    ../../gv/src/gui/qt/RTreePropertyBrowser.cpp \
    ../../gv/src/gui/qt/RGraphicsSceneGl.cpp \
    ../../gv/src/gui/qt/RGraphicsViewGl.cpp \
    ../../gv/src/gui/qt/RGraphicsViewGl3d.cpp \
    ../../gv/src/gui/qt/RHelpBrowser.cpp \
    ../../gv/src/gui/qt/RGuiActionQt.cpp \
    ../../gv/src/gui/qt/RMainWindowQt.cpp \
    ../../gv/src/gui/qt/RGraphicsViewQt.cpp \
    ../../gv/src/gui/qt/RGraphicsSceneQt.cpp \
    ../../gv/src/gui/qt/RWebView.cpp \
    ../../gv/src/gui/qt/qgraphicsview/RGraphicsViewQV.cpp \
    ../../gv/src/gui/qt/qgraphicsview/RGraphicsSceneQV.cpp \
    ../../gv/src/gui/qt/qgraphicsview/RGraphicsLineItem.cpp \
    ../../gv/src/gui/qt/qgraphicsview/RGraphicsItem.cpp
HEADERS += \
    ../../gv/src/gui/qt/RListView.h \
    ../../gv/src/gui/qt/RFileSystemModel.h \
    ../../gv/src/gui/qt/RFontChooserWidget.h \
    ../../gv/src/gui/qt/RRulerQt.h \
    ../../gv/src/gui/qt/RLineweightEditorFactory.h \
    ../../gv/src/gui/qt/RLineweightPropertyManager.h \
    ../../gv/src/gui/qt/RLinetypePropertyManager.h \
    ../../gv/src/gui/qt/RLinetypeEditorFactory.h \
    ../../gv/src/gui/qt/RAbstractEditorFactory.h \
    ../../gv/src/gui/qt/RColorPropertyManager.h \
    ../../gv/src/gui/qt/RColorEditorFactory.h \
    ../../gv/src/gui/qt/RLinetypeCombo.h \
    ../../gv/src/gui/qt/RLineweightCombo.h \
    ../../gv/src/gui/qt/RColorCombo.h \
    ../../gv/src/gui/qt/RListWidget.h \
    ../../gv/src/gui/qt/RCommandLine.h \
    ../../gv/src/gui/qt/RMdiChildQt.h \
    ../../gv/src/gui/qt/RTextEdit.h \
    ../../gv/src/gui/qt/RMathLineEdit.h \
    ../../gv/src/gui/qt/RTreePropertyBrowser.h \
    ../../gv/src/gui/qt/RGraphicsSceneGl.h \
    ../../gv/src/gui/qt/RGraphicsViewGl.h \
    ../../gv/src/gui/qt/RGraphicsViewGl3d.h \
    ../../gv/src/gui/qt/RHelpBrowser.h \
    ../../gv/src/gui/qt/RGuiActionQt.h \
    ../../gv/src/gui/qt/RMainWindowQt.h \
    ../../gv/src/gui/qt/RGraphicsViewQt.h \
    ../../gv/src/gui/qt/RGraphicsSceneQt.h \
    ../../gv/src/gui/qt/RWebView.h \
    ../../gv/src/gui/qt/qgraphicsview/RGraphicsViewQV.h \
    ../../gv/src/gui/qt/qgraphicsview/RGraphicsSceneQV.h \
    ../../gv/src/gui/qt/qgraphicsview/RGraphicsLineItem.h \
    ../../gv/src/gui/qt/qgraphicsview/RGraphicsItem.h
#TEMPLATE = lib
#CONFIG += staticlib
#TARGET = guiqt
