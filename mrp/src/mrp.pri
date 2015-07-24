#-------------------------------------------------
# $Id: sail.pri 1678 2012-06-20 20:32:50Z rutger $
# Project created 2012-07-26T14:00:00
# - for building plugin MRP
# - ../src path is required to use .pri file also from ../test
# ----------------------------------------------------------

HEADERS += \
    ../../acc/src/model/acc_contact.h \
    ../../acc/src/model/acc_customer.h \
    ../../acc/src/model/acc_project.h \
    ../../acc/src/model/acc_salesorder.h \
    ../src/actions/mrp_actionclose.h \
    ../src/actions/mrp_actionfactory.h \
    ../src/actions/mrp_actionplugin.h \
    ../src/actions/mrp_actionselectproject.h \
    ../src/actions/mrp_actionsoftwarelicense.h \
    ../src/actions/mrp_actionsoftwarelicensereport.h \
    ../src/gui/mrp_dialogfactory.h \
    ../src/gui/mrp_selectcontactdialog.h \
    ../src/gui/mrp_selectcustomerdialog.h \
    ../src/gui/mrp_selectorderdialog.h \
    ../src/gui/mrp_selectprojectdialog.h \
    ../src/gui/mrp_softwarelicensereportwidget.h \
    ../src/gui/mrp_softwarelicensewidget.h \
    ../src/model/mrp_modelfactory.h \
    ../src/model/mrp_objectfactory.h \
    ../src/model/mrp_softwarelicense.h \
    ../src/operations/mrp_softwarelicenseoperation.h \
    ../src/utils/mrp_sqlcommonfunctions.h

SOURCES += \
    ../../acc/src/model/acc_contact.cpp \
    ../../acc/src/model/acc_customer.cpp \
    ../../acc/src/model/acc_project.cpp \
    ../../acc/src/model/acc_salesorder.cpp \
    ../src/actions/mrp_actionclose.cpp \
    ../src/actions/mrp_actionfactory.cpp \
    ../src/actions/mrp_actionplugin.cpp \
    ../src/actions/mrp_actionselectproject.cpp \
    ../src/actions/mrp_actionsoftwarelicense.cpp \
    ../src/actions/mrp_actionsoftwarelicensereport.cpp \
    ../src/gui/mrp_dialogfactory.cpp \
    ../src/gui/mrp_softwarelicensereportwidget.cpp \
    ../src/gui/mrp_softwarelicensewidget.cpp \
    ../src/model/mrp_modelfactory.cpp \
    ../src/model/mrp_objectfactory.cpp \
    ../src/model/mrp_softwarelicense.cpp \
    ../src/operations/mrp_softwarelicenseoperation.cpp \
    ../src/utils/mrp_sqlcommonfunctions.cpp

FORMS += \
    ../src/gui/mrp_softwarelicensereportwidget.ui \
    ../src/gui/mrp_softwarelicensewidget.ui

RESOURCES += \
    ../../db/src/gui/db.qrc

OTHER_FILES += \
    ../src/actions/mrp_actionplugin.json \
    ../src/gui/mrp_index.dox \
    ../src/gui/mrp_selectprojectdialog.dox \
    ../src/gui/mrp_softwarelicensereportwidget.dox \
    ../src/gui/mrp_softwarelicensewidget.dox

