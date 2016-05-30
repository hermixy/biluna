# ----------------------------------------------------------
# $Id: dbtest.pro 1972 2013-08-19 07:42:21Z rutger $
# dbtest.pro
# - for testing of DB library only
# ----------------------------------------------------------
#TEMPLATE = lib
TEMPLATE = app
LANGUAGE = C++
#TARGET = db
TARGET   = dbtest
DESTDIR  = ./

CONFIG += openssl-linked
CONFIG += shared

# Biluna specific
#CONFIG += RB_DYNLIB
# Has impact in global.h
DEFINES += DB_DLL
# Creates additional test objects
DEFINES += DB_TEST

# For modeltest, dynamictreemodel to use the test framework
QT += testlib

# Compiler settings for release, debug and path for ui, moc, binary files
exists( ../../mkspecs/defs.pro ) {
    include( ../../mkspecs/defs.pro )
}
else {
    # message and exit
    error( "An error has occurred, ../../mkspecs/defs.pro is missing." )
}

INCLUDEPATH += \
    C:/Qt/Qt5.6.0/5.6/Src/qtbase/tests/auto/other/modeltest \
    actions \
    entities \
    filters \
    gui \
    gui/ui \
    model \
    operations \
    utils \
    visitors \
    ../../db/src/actions \
    ../../db/src/entities \
    ../../db/src/filters \
    ../../db/src/gui \
#    ../../db/src/gui/ui \
    ../../db/src/model \
    ../../db/src/operations \
    ../../db/src/utils \
    ../../db/src/visitors \
    ../../thrd/browser \
    ../../thrd/browser/data \
    ../../thrd/browser/htmls \
    ../../thrd/nightcharts \
    ../../thrd/qhexedit2/src \
    ../../thrd/speedcrunch-0.10.1/src \
    ../../thrd/speedcrunch-0.10.1/src/3rdparty \
    ../../thrd/speedcrunch-0.10.1/src/3rdparty/util \
    ../../thrd/speedcrunch-0.10.1/src/base \
#    ../../thrd/speedcrunch-0.10.1/src/books \
    ../../thrd/speedcrunch-0.10.1/src/gui \
#    ../../thrd/speedcrunch-0.10.1/src/i18n \
#    ../../thrd/speedcrunch-0.10.1/src/main \
    ../../thrd/speedcrunch-0.10.1/src/math \
    ../../thrd/speedcrunch-0.10.1/src/resources
#    ../../thrd/speedcrunch-0.10.1/src/test \

# LIBS		+=  -lmysqlclient -L/usr/lib/mysql/

# Add forms, headers and sources
exists( ../../db/src/db.pri) {
    include( ../../db/src/db.pri )
} else {
    # message and exit
    error( "An error has occurred, db.pri is missing." )
}

# In case of release remove header and source debug files from .pri
#CONFIG (release) {
#    HEADERS -= \
#        ../src/utils/rb_signalspydialog.h
#    SOURCES -= \
#        ../src/utils/rb_signalspydialog.cpp
#}

# Browser, Nightcharts, Speedcrunch calculator
exists( ../../db/src/thrd.pri ) {
    include( ../../db/src/thrd.pri )
} else {
    error( "An error has occurred, thrd.pri is missing." )
}

FORMS += \
    ../../db/test/gui/db_testdbobjectwidget.ui \
    ../../db/test/gui/db_testtablewidget.ui \
    ../../db/test/gui/db_testtabletreewidget.ui \
    ../../db/test/gui/db_testtextwidget.ui \
    ../../db/test/gui/db_testtreewidget.ui

HEADERS += \
    C:/Qt/Qt5.6.0/5.6/Src/qtbase/tests/auto/other/modeltest/dynamictreemodel.h \
    C:/Qt/Qt5.6.0/5.6/Src/qtbase/tests/auto/other/modeltest/modeltest.h \
    ../../db/test/actions/db_actiontest.h \
    ../../db/test/actions/db_actiontestcleartext.h \
    ../../db/test/actions/db_actiontestdatabasemodel.h \
    ../../db/test/actions/db_actiontestdatabasetreemodel.h \
    ../../db/test/actions/db_actiontestdbobject.h \
    ../../db/test/actions/db_actiontestdbtabletree.h \
    ../../db/test/actions/db_actiontestfactory.h \
    ../../db/test/actions/db_actiontestinmemorymodel.h \
    ../../db/test/actions/db_actiontestinmemorytreemodel.h \
    ../../db/test/actions/db_actiontestobject.h \
    ../../db/test/actions/db_actiontestrelation.h \
    ../../db/test/actions/db_actiontestsystemselectproject.h \
    ../../db/test/actions/db_actiontoolsexportmodel.h \
    ../../db/test/actions/db_actiontoolsplugins.h \
    ../../db/test/actions/db_actiontoolssignalspy.h \
    ../../db/test/actions/db_testactionfactory.h \
    ../../db/test/gui/db_testdbobjectwidget.h \
#    ../../db/test/gui/db_testdialogfactory.h \
    ../../db/test/gui/db_testmainwindow.h \
    ../../db/test/gui/db_testrelationwidget.h \
    ../../db/test/gui/db_testselectprojectwidget.h \
    ../../db/test/gui/db_testtablewidget.h \
    ../../db/test/gui/db_testtabletreewidget.h \
    ../../db/test/gui/db_testtextwidget.h \
    ../../db/test/gui/db_testtreewidget.h \
    ../../db/test/gui/main.h \
    ../../db/test/model/db_test.h \
    ../../db/test/model/db_testchild.h \
    ../../db/test/model/db_testproject.h \
    ../../db/test/model/db_testrelation.h

SOURCES += \
    C:/Qt/Qt5.6.0/5.6/Src/qtbase/tests/auto/other/modeltest/dynamictreemodel.cpp \
    C:/Qt/Qt5.6.0/5.6/Src/qtbase/tests/auto/other/modeltest/modeltest.cpp \
    ../../db/test/actions/db_actiontest.cpp \
    ../../db/test/actions/db_actiontestcleartext.cpp \
    ../../db/test/actions/db_actiontestdatabasemodel.cpp \
    ../../db/test/actions/db_actiontestdatabasetreemodel.cpp \
    ../../db/test/actions/db_actiontestdbtabletree.cpp \
    ../../db/test/actions/db_actiontestdbobject.cpp \
    ../../db/test/actions/db_actiontestfactory.cpp \
    ../../db/test/actions/db_actiontestinmemorymodel.cpp \
    ../../db/test/actions/db_actiontestinmemorytreemodel.cpp \
    ../../db/test/actions/db_actiontestobject.cpp \
    ../../db/test/actions/db_actiontestrelation.cpp \
    ../../db/test/actions/db_actiontestsystemselectproject.cpp \
    ../../db/test/actions/db_actiontoolsexportmodel.cpp \
    ../../db/test/actions/db_actiontoolsplugins.cpp \
    ../../db/test/actions/db_actiontoolssignalspy.cpp \
    ../../db/test/actions/db_testactionfactory.cpp \
    ../../db/test/gui/db_testdbobjectwidget.cpp \
#    ../../db/test/gui/db_testdialogfactory.cpp \
    ../../db/test/gui/db_testmainwindow.cpp \
    ../../db/test/gui/db_testrelationwidget.cpp \
    ../../db/test/gui/db_testselectprojectwidget.cpp \
    ../../db/test/gui/db_testtablewidget.cpp \
    ../../db/test/gui/db_testtabletreewidget.cpp \
    ../../db/test/gui/db_testtextwidget.cpp \
    ../../db/test/gui/db_testtreewidget.cpp \
    ../../db/test/gui/main.cpp \
    ../../db/test/model/db_test.cpp \
    ../../db/test/model/db_testchild.cpp \
    ../../db/test/model/db_testproject.cpp \
    ../../db/test/model/db_testrelation.cpp
