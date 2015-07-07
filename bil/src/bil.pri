#-------------------------------------------------
# $Id: acc.pri 1329 2010-12-11 18:27:47Z rutger $
# Project created by QtCreator 2009-12-18T18:11:30
# - for building accounting application ACC
# - ../src path is required to use .pri file also from ../test
# ----------------------------------------------------------

HEADERS += \
#    ../../thrd/qtsingleapplication/src/qtlocalpeer.h \
#    ../../thrd/qtsingleapplication/src/qtsingleapplication.h \
    ../src/gui/bil_mainwindow.h \
    ../src/gui/main.h

SOURCES += \
#    ../../thrd/qtsingleapplication/src/qtlocalpeer.cpp \
#    ../../thrd/qtsingleapplication/src/qtsingleapplication.cpp \
    ../src/gui/bil_mainwindow.cpp \
    ../src/gui/main.cpp

#FORMS += \
#    gui/acc_mainwindow.ui

RESOURCES += \
    ../src/gui/bil.qrc \
    ../../db/src/gui/db.qrc

OTHER_FILES += \
    ../src/gui/bil_index.dox \
    ../src/gui/bil_installationandfirststeps.dox \
    ../src/gui/bil_introduction.dox \
    ../src/gui/home.dox

