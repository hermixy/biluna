#-------------------------------------------------
# $Id: sail.pri 1678 2012-06-20 20:32:50Z rutger $
# Project created 2012-07-26T14:00:00
# - for building sailing plugin SAIL
# - ../src path is required to use .pri file also from ../test
# ----------------------------------------------------------

HEADERS += \
    ../src/actions/sail_actionclose.h \
    ../src/actions/sail_actionfactory.h \
    ../src/actions/sail_actionmapnavigation.h \
    ../src/actions/sail_actionplugin.h \
    ../src/actions/sail_actionpolardiagram.h \
    ../src/actions/sail_actionprojectedit.h \
    ../src/actions/sail_actionselectproject.h \
    ../src/actions/sail_actiontrackdistancereport.h \
    ../src/gui/sail_dialogfactory.h \
    ../src/gui/sail_mapnavigationwidget.h \
    ../src/gui/sail_polardiagramwidget.h \
    ../src/gui/sail_projectdialog.h \
    ../src/gui/sail_projecteditdialog.h \
    ../src/gui/sail_selectcoordinatedialog.h \
    ../src/gui/sail_selectmapdialog.h \
    ../src/gui/sail_selectsymboldialog.h \
    ../src/gui/sail_trackdistancereportwidget.h \
    ../src/model/sail_coordinate.h \
    ../src/model/sail_map.h \
    ../src/model/sail_modelfactory.h \
    ../src/model/sail_objectfactory.h \
    ../src/model/sail_project.h \
    ../src/model/sail_route.h \
    ../src/model/sail_routecoordinate.h \
    ../src/model/sail_symbol.h \
#    ../src/model/sail_syssetting.h \
    ../src/model/sail_track.h \
    ../src/operations/sail_routeoperation.h \
    ../src/operations/sail_trackoperation.h \
    ../src/utils/sail_coordcalc.h \
    ../src/utils/sail_sqlcommonfunctions.h

SOURCES += \
    ../src/actions/sail_actionclose.cpp \
    ../src/actions/sail_actionfactory.cpp \
    ../src/actions/sail_actionmapnavigation.cpp \
    ../src/actions/sail_actionplugin.cpp \
    ../src/actions/sail_actionpolardiagram.cpp \
    ../src/actions/sail_actionprojectedit.cpp \
    ../src/actions/sail_actionselectproject.cpp \
    ../src/actions/sail_actiontrackdistancereport.cpp \
    ../src/gui/sail_dialogfactory.cpp \
    ../src/gui/sail_mapnavigationwidget.cpp \
    ../src/gui/sail_polardiagramwidget.cpp \
    ../src/gui/sail_projecteditdialog.cpp \
    ../src/gui/sail_trackdistancereportwidget.cpp \
    ../src/model/sail_coordinate.cpp \
    ../src/model/sail_map.cpp \
    ../src/model/sail_modelfactory.cpp \
    ../src/model/sail_objectfactory.cpp \
    ../src/model/sail_project.cpp \
    ../src/model/sail_route.cpp \
    ../src/model/sail_routecoordinate.cpp \
    ../src/model/sail_symbol.cpp \
#    ../src/model/sail_syssetting.cpp \
    ../src/model/sail_track.cpp \
    ../src/operations/sail_routeoperation.cpp \
    ../src/operations/sail_trackoperation.cpp \
    ../src/utils/sail_coordcalc.cpp \
    ../src/utils/sail_sqlcommonfunctions.cpp

FORMS += \
    ../src/gui/sail_mapnavigationwidget.ui \
    ../src/gui/sail_trackdistancereportwidget.ui

RESOURCES += \
#    ../../acc/src/gui/acc.qrc \
    ../../db/src/gui/db.qrc \
    ../../sail/src/gui/sail.qrc

OTHER_FILES += \
    ../src/actions/sail_actionplugin.json \
    ../src/gui/sail_index.dox \
    ../src/gui/sail_mapnavigationwidget.dox \
    ../src/gui/sail_polardiagramwidget.dox \
    ../src/gui/sail_projectdialog.dox \
    ../src/gui/sail_projecteditdialog.dox \
    ../src/gui/sail_trackdistancereportwidget.dox

