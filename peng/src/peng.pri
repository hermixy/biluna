#-------------------------------------------------
# $Id: acc.pri 1352 2011-01-26 22:01:25Z rutger $
# Project created 2011-02-15
# - for building Plant Engineering 2D PENG plugin
# - ../src path is required to use .pri file also from ../test
# ----------------------------------------------------------

FORMS += \
    ../src/gui/peng_conestrikeoutwidget.ui \
#    ../src/gui/peng_equipmentmasterwidget.ui \
    ../src/gui/peng_insulationdialog.ui \
    ../src/gui/peng_linedetailwidget.ui \
#    ../src/gui/peng_linemasterwidget.ui \
    ../src/gui/peng_navigationwidget.ui \
    ../src/gui/peng_paintdialog.ui \
    ../src/gui/peng_pedeqsummarywidget.ui \
    ../src/gui/peng_pedwidget.ui \
    ../src/gui/peng_pipeclassdialog.ui \
    ../src/gui/peng_settingsdialog.ui \
    ../src/gui/peng_weldoletweldvolumewidget.ui

HEADERS += \
    ../src/actions/peng_actionclose.h \
    ../src/actions/peng_actionconestrikeout.h \
    ../src/actions/peng_actioneditproject.h \
    ../src/actions/peng_actioneqtype.h \
    ../src/actions/peng_actionequipment.h \
    ../src/actions/peng_actionfactory.h \
    ../src/actions/peng_actionfluid.h \
    ../src/actions/peng_actionfluidstability.h \
    ../src/actions/peng_actionhazardgroup.h \
    ../src/actions/peng_actioninsulation.h \
    ../src/actions/peng_actionlinedetail.h \
    ../src/actions/peng_actionpaint.h \
    ../src/actions/peng_actionpedclassification.h \
    ../src/actions/peng_actionpedeqsummary.h \
    ../src/actions/peng_actionpipeclass.h \
    ../src/actions/peng_actionplugin.h \
    ../src/actions/peng_actionselectproject.h \
    ../src/actions/peng_actionsettings.h \
    ../src/actions/peng_actionweldvolcalc.h \
    ../src/gui/peng_conestrikeoutwidget.h \
    ../src/gui/peng_dialogfactory.h \
#    ../src/gui/peng_equipmentmasterwidget.h \
    ../src/gui/peng_fluiddialog.h \
    ../src/gui/peng_fluidstabilitydialog.h \
    ../src/gui/peng_graphicsview.h \
    ../src/gui/peng_hazardgroupdialog.h \
    ../src/gui/peng_insulationdialog.h \
    ../src/gui/peng_linedetailwidget.h \
#    ../src/gui/peng_linemasterwidget.h \
    ../src/gui/peng_navigationwidget.h \
    ../src/gui/peng_paintdialog.h \
    ../src/gui/peng_pedeqsummarywidget.h \
    ../src/gui/peng_pedtable.h \
    ../src/gui/peng_pedwidget.h \
    ../src/gui/peng_pipeclassdialog.h \
    ../src/gui/peng_projectdialog.h \
    ../src/gui/peng_projecteditdialog.h \
    ../src/gui/peng_settingsdialog.h \
    ../src/gui/peng_typeeditdialog.h \
    ../src/gui/peng_typeselectdialog.h \
    ../src/gui/peng_weldoletweldvolumewidget.h \
    ../src/math/peng_vector.h \
    ../src/model/peng.h \
    ../src/model/peng_component.h \
    ../src/model/peng_cone.h \
    ../src/model/peng_equipment.h \
    ../src/model/peng_fluid.h \
    ../src/model/peng_fluidstability.h \
    ../src/model/peng_hazardgroup.h \
    ../src/model/peng_insulationcc.h \
    ../src/model/peng_insulationhci.h \
    ../src/model/peng_insulationhco.h \
    ../src/model/peng_insulationpp.h \
    ../src/model/peng_lineclassif.h \
    ../src/model/peng_linecustom.h \
    ../src/model/peng_lineengindes.h \
    ../src/model/peng_lineprocess.h \
    ../src/model/peng_modelfactory.h \
    ../src/model/peng_objectfactory.h \
    ../src/model/peng_paint.h \
    ../src/model/peng_pedclassif.h \
    ../src/model/peng_pipeclass.h \
    ../src/model/peng_pipeclasspt.h \
    ../src/model/peng_pipeclasssize.h \
    ../src/model/peng_project.h \
    ../src/model/peng_setting.h \
    ../src/model/peng_type.h \
    ../src/model/peng_typemember.h \
    ../src/model/peng_typesection.h \
    ../src/model/peng_typesubsection.h \
    ../src/model/peng_weldolet.h \
    ../src/operations/peng_determinepedclass.h \
    ../src/utils/peng_sqlcommonfunctions.h

SOURCES += \
    ../src/actions/peng_actionclose.cpp \
    ../src/actions/peng_actionconestrikeout.cpp \
    ../src/actions/peng_actioneditproject.cpp \
    ../src/actions/peng_actioneqtype.cpp \
    ../src/actions/peng_actionequipment.cpp \
    ../src/actions/peng_actionfactory.cpp \
    ../src/actions/peng_actionfluid.cpp \
    ../src/actions/peng_actionfluidstability.cpp \
    ../src/actions/peng_actionhazardgroup.cpp \
    ../src/actions/peng_actioninsulation.cpp \
    ../src/actions/peng_actionlinedetail.cpp \
    ../src/actions/peng_actionpaint.cpp \
    ../src/actions/peng_actionpedclassification.cpp \
    ../src/actions/peng_actionpedeqsummary.cpp \
    ../src/actions/peng_actionpipeclass.cpp \
    ../src/actions/peng_actionplugin.cpp \
    ../src/actions/peng_actionsettings.cpp \
    ../src/actions/peng_actionselectproject.cpp \
    ../src/actions/peng_actionweldvolcalc.cpp \
    ../src/gui/peng_conestrikeoutwidget.cpp \
    ../src/gui/peng_dialogfactory.cpp \
#    ../src/gui/peng_equipmentmasterwidget.cpp \
    ../src/gui/peng_graphicsview.cpp \
    ../src/gui/peng_insulationdialog.cpp \
    ../src/gui/peng_linedetailwidget.cpp \
#    ../src/gui/peng_linemasterwidget.cpp \
    ../src/gui/peng_navigationwidget.cpp \
    ../src/gui/peng_paintdialog.cpp \
    ../src/gui/peng_pedeqsummarywidget.cpp \
    ../src/gui/peng_pedtable.cpp \
    ../src/gui/peng_pedwidget.cpp \
    ../src/gui/peng_pipeclassdialog.cpp \
    ../src/gui/peng_projecteditdialog.cpp \
    ../src/gui/peng_settingsdialog.cpp \
    ../src/gui/peng_weldoletweldvolumewidget.cpp \
    ../src/math/peng_vector.cpp \
    ../src/model/peng_component.cpp \
    ../src/model/peng_cone.cpp \
    ../src/model/peng_equipment.cpp \
    ../src/model/peng_fluid.cpp \
    ../src/model/peng_fluidstability.cpp \
    ../src/model/peng_hazardgroup.cpp \
    ../src/model/peng_insulationcc.cpp \
    ../src/model/peng_insulationhci.cpp \
    ../src/model/peng_insulationhco.cpp \
    ../src/model/peng_insulationpp.cpp \
    ../src/model/peng_line.cpp \
    ../src/model/peng_lineclassif.cpp \
    ../src/model/peng_linecustom.cpp \
    ../src/model/peng_lineengindes.cpp \
    ../src/model/peng_lineprocess.cpp \
    ../src/model/peng_modelfactory.cpp \
    ../src/model/peng_objectfactory.cpp \
    ../src/model/peng_paint.cpp \
    ../src/model/peng_pedclassif.cpp \
    ../src/model/peng_pipeclass.cpp \
    ../src/model/peng_pipeclasspt.cpp \
    ../src/model/peng_pipeclasssize.cpp \
    ../src/model/peng_project.cpp \
    ../src/model/peng_setting.cpp \
    ../src/model/peng_type.cpp \
    ../src/model/peng_typemember.cpp \
    ../src/model/peng_typesection.cpp \
    ../src/model/peng_typesubsection.cpp \
    ../src/model/peng_weldolet.cpp \
    ../src/operations/peng_determinepedclass.cpp \
    ../src/utils/peng_sqlcommonfunctions.cpp

RESOURCES += \
    ../src/gui/peng.qrc \
    ../../db/src/gui/db.qrc

OTHER_FILES += \
    ../src/actions/peng_actionplugin.json \
    ../src/gui/peng_conestrikeoutwidget.dox \
    ../src/gui/peng_fluiddialog.dox \
    ../src/gui/peng_fluidstabilitydialog.dox \
    ../src/gui/peng_index.dox \
    ../src/gui/peng_insulationdialog.dox \
    ../src/gui/peng_linedetailwidget.dox \
    ../src/gui/peng_paintdialog.dox \
    ../src/gui/peng_ped.dox \
    ../src/gui/peng_ped_guide.dox \
    ../src/gui/peng_pedeqsummarywidget.dox \
    ../src/gui/peng_pedwidget.dox \
    ../src/gui/peng_pipeclassdialog.dox \
    ../src/gui/peng_projecteditdialog.dox \
    ../src/gui/peng_settingsdialog.dox \
    ../src/gui/peng_weldoletweldvolumewidget.dox
