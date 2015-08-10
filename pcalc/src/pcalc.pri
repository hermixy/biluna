#-------------------------------------------------
# $Id: pcalc.pri 1352 2011-01-26 22:01:25Z rutger $
# Project created 2015-02-25
# - for building calculations in PCALC
# - ../src path is required to use .pri file also from ../test
# ----------------------------------------------------------

FORMS += \
    ../src/gui/pcalc_en1591widget.ui

HEADERS += \
    ../../peng/src/model/peng_project.h \
    ../src/actions/pcalc_actionclose.h \
    ../src/actions/pcalc_actioneditproject.h \
    ../src/actions/pcalc_actionen1591flange.h \
    ../src/actions/pcalc_actionfactory.h \
    ../src/actions/pcalc_actionplugin.h \
    ../src/actions/pcalc_actionselectproject.h \
    ../src/calc/en1591handler.h \
    ../src/calc/EN1591/assembly.h \
    ../src/calc/EN1591/bolt.h \
    ../src/calc/EN1591/bolthole.h \
    ../src/calc/EN1591/calculator.h \
#    ../src/calc/EN1591/Document.h \
    ../src/calc/EN1591/flange.h \
    ../src/calc/EN1591/flange_blind.h \
    ../src/calc/EN1591/flange_integral.h \
    ../src/calc/EN1591/flange_loose.h \
    ../src/calc/EN1591/force.h \
    ../src/calc/EN1591/gasket.h \
    ../src/calc/EN1591/loadcase.h \
    ../src/calc/EN1591/loadcaselist.h \
    ../src/calc/EN1591/shell.h \
    ../src/calc/EN1591/table02_15property.h \
    ../src/calc/EN1591/table16property.h \
    ../src/calc/EN1591/table17_30property.h \
    ../src/calc/EN1591/tableeproperty.h \
    ../src/calc/EN1591/tablegproperty.h \
    ../src/calc/EN1591/tablegsimple.h \
    ../src/calc/EN1591/washer.h \
    ../src/calc/EN1591/unittest/assemblytest.h \
    ../src/calc/EN1591/unittest/bolttest.h \
    ../src/calc/EN1591/unittest/flange_blindtest.h \
    ../src/calc/EN1591/unittest/flange_integraltest.h \
    ../src/calc/EN1591/unittest/flange_loosetest.h \
    ../src/calc/EN1591/unittest/flangetest.h \
    ../src/calc/EN1591/unittest/forcetest.h \
    ../src/calc/EN1591/unittest/gaskettest.h \
    ../src/calc/EN1591/unittest/tablegsimpletest.h \
    ../src/calc/EN1591/unittest/washertest.h \
    ../src/calc/pcalc_inputoutput.h \
    ../src/calc/rb_allowanceservice.h \
    ../src/calc/rb_materialservice.h \
    ../src/calc/rb_namespace.h \
    ../src/calc/rb_qualityservice.h \
    ../src/calc/rb_report.h \
    ../src/calc/rb_tablemath.h \
    ../src/calc/unittest/rb_tablemathtest.h \
    ../src/calc/unittest/rb_unittest.h \
    ../src/calc/unittest/rb_unittestfactory.h \
    ../src/calc/unittest/en1591_unittestfactory.h \
    ../src/gui/pcalc_dialogfactory.h \
    ../src/gui/pcalc_en1591widget.h \
    ../src/gui/pcalc_projectdialog.h \
    ../src/gui/pcalc_projecteditdialog.h \
    ../src/model/EN1591/pcalc_en1591_assembly.h \
    ../src/model/EN1591/pcalc_en1591_boltnutwasher.h \
    ../src/model/EN1591/pcalc_en1591_flange.h \
    ../src/model/EN1591/pcalc_en1591_gasket.h \
    ../src/model/EN1591/pcalc_en1591_loadcase.h \
    ../src/model/EN1591/pcalc_en1591_shell.h \
    ../src/model/pcalc.h \
    ../src/model/pcalc_modelfactory.h \
    ../src/model/pcalc_objectfactory.h \
    ../src/utils/pcalc_sqlcommonfunctions.h

SOURCES += \
    ../../peng/src/model/peng_project.cpp \
    ../src/actions/pcalc_actionclose.cpp \
    ../src/actions/pcalc_actioneditproject.cpp \
    ../src/actions/pcalc_actionen1591flange.cpp \
    ../src/actions/pcalc_actionfactory.cpp \
    ../src/actions/pcalc_actionplugin.cpp \
    ../src/actions/pcalc_actionselectproject.cpp \
    ../src/calc/en1591handler.cpp \
    ../src/calc/EN1591/assembly.cpp \
    ../src/calc/EN1591/bolt.cpp \
    ../src/calc/EN1591/bolthole.cpp \
    ../src/calc/EN1591/calculator.cpp \
#    ../src/calc/EN1591/Document.cpp \
    ../src/calc/EN1591/flange.cpp \
    ../src/calc/EN1591/flange_blind.cpp \
    ../src/calc/EN1591/flange_integral.cpp \
    ../src/calc/EN1591/flange_loose.cpp \
    ../src/calc/EN1591/force.cpp \
    ../src/calc/EN1591/gasket.cpp \
    ../src/calc/EN1591/loadcase.cpp \
    ../src/calc/EN1591/loadcaselist.cpp \
    ../src/calc/EN1591/shell.cpp \
    ../src/calc/EN1591/table02_15property.cpp \
    ../src/calc/EN1591/table16property.cpp \
    ../src/calc/EN1591/table17_30property.cpp \
    ../src/calc/EN1591/tableeproperty.cpp \
    ../src/calc/EN1591/tablegproperty.cpp \
    ../src/calc/EN1591/tablegsimple.cpp \
    ../src/calc/EN1591/washer.cpp \
    ../src/calc/EN1591/unittest/assemblytest.cpp \
    ../src/calc/EN1591/unittest/bolttest.cpp \
    ../src/calc/EN1591/unittest/flange_blindtest.cpp \
    ../src/calc/EN1591/unittest/flange_integraltest.cpp \
    ../src/calc/EN1591/unittest/flange_loosetest.cpp \
    ../src/calc/EN1591/unittest/flangetest.cpp \
    ../src/calc/EN1591/unittest/forcetest.cpp \
    ../src/calc/EN1591/unittest/gaskettest.cpp \
    ../src/calc/EN1591/unittest/tablegsimpletest.cpp \
    ../src/calc/EN1591/unittest/washertest.cpp \
    ../src/calc/pcalc_inputoutput.cpp \
    ../src/calc/rb_allowanceservice.cpp \
    ../src/calc/rb_materialservice.cpp \
    ../src/calc/rb_qualityservice.cpp \
    ../src/calc/rb_report.cpp \
    ../src/calc/rb_tablemath.cpp \
    ../src/calc/unittest/rb_tablemathtest.cpp \
    ../src/calc/unittest/rb_unittest.cpp \
    ../src/calc/unittest/rb_unittestfactory.cpp \
    ../src/calc/unittest/en1591_unittestfactory.cpp \
    ../src/gui/pcalc_dialogfactory.cpp \
    ../src/gui/pcalc_en1591widget.cpp \
    ../src/gui/pcalc_projecteditdialog.cpp \
    ../src/model/EN1591/pcalc_en1591_assembly.cpp \
    ../src/model/EN1591/pcalc_en1591_boltnutwasher.cpp \
    ../src/model/EN1591/pcalc_en1591_flange.cpp \
    ../src/model/EN1591/pcalc_en1591_gasket.cpp \
    ../src/model/EN1591/pcalc_en1591_loadcase.cpp \
    ../src/model/EN1591/pcalc_en1591_shell.cpp \
    ../src/model/pcalc_modelfactory.cpp \
    ../src/model/pcalc_objectfactory.cpp \
    ../src/utils/pcalc_sqlcommonfunctions.cpp

RESOURCES += \

OTHER_FILES += \
    ../src/actions/pcalc_actionplugin.json \
    ../src/gui/pcalc_en1591widget.dox
