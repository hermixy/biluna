#-------------------------------------------------
# $Id: pcalc.pri 1352 2011-01-26 22:01:25Z rutger $
# Project created 2015-02-25
# - for building calculations in PCALC
# - ../src path is required to use .pri file also from ../test
# ----------------------------------------------------------

FORMS += \
    ../src/gui/pcalc_classgeneratorwidget.ui \
    ../src/gui/pcalc_en1591selectgasketwidget.ui \
    ../src/gui/pcalc_en1591widget.ui \
    ../src/gui/rb_databasetablewidget.ui

HEADERS += \
    ../../peng/src/model/peng_project.h \
    ../src/actions/pcalc_actionclose.h \
    ../src/actions/pcalc_actioneditproject.h \
    ../src/actions/pcalc_actionen1591flange.h \
    ../src/actions/pcalc_actionengasket.h \
    ../src/actions/pcalc_actionfactory.h \
    ../src/actions/pcalc_actionplugin.h \
    ../src/actions/pcalc_actionselectproject.h \
    ../src/calc/en1591handler.h \
    ../src/calc/EN1591/assembly.h \
    ../src/calc/EN1591/bolt.h \
    ../src/calc/EN1591/bolthole.h \
    ../src/calc/EN1591/calculator.h \
    ../src/calc/EN1591/flange.h \
    ../src/calc/EN1591/flange_blind.h \
    ../src/calc/EN1591/flange_integral.h \
    ../src/calc/EN1591/flange_loose.h \
    ../src/calc/EN1591/force.h \
    ../src/calc/EN1591/gasket.h \
    ../src/calc/EN1591/loadcase.h \
    ../src/calc/EN1591/loadcaselist.h \
    ../src/calc/EN1591/shell.h \
    ../src/calc/EN1591/washer.h \
    ../src/calc/EN1591/unittest/assemblytest.h \
    ../src/calc/EN1591/unittest/bolttest.h \
    ../src/calc/EN1591/unittest/flange_blindtest.h \
    ../src/calc/EN1591/unittest/flange_integraltest.h \
    ../src/calc/EN1591/unittest/flange_loosetest.h \
    ../src/calc/EN1591/unittest/flangetest.h \
    ../src/calc/EN1591/unittest/forcetest.h \
    ../src/calc/EN1591/unittest/gaskettest.h \
    ../src/calc/EN1591/unittest/washertest.h \
    ../src/calc/pcalc_inputoutput.h \
    ../src/calc/rb_allowanceservice.h \
    ../src/calc/rb_materialservice.h \
    ../src/calc/rb_namespace.h \
    ../src/calc/rb_qualityservice.h \
    ../src/calc/unittest/rb_tablemathtest.h \
    ../src/calc/unittest/rb_unittest.h \
    ../src/calc/unittest/rb_unittestfactory.h \
    ../src/calc/unittest/en1591_unittestfactory.h \
    ../src/gui/pcalc_classgeneratorwidget.h \
    ../src/gui/pcalc_dialogfactory.h \
    ../src/gui/pcalc_en13555gasketwidget.h \
    ../src/gui/pcalc_en13555manufwidget.h \
    ../src/gui/pcalc_en13555typewidget.h \
    ../src/gui/pcalc_en1591selectgasketwidget.h \
    ../src/gui/pcalc_en1591widget.h \
    ../src/gui/pcalc_projectdialog.h \
    ../src/gui/pcalc_projecteditdialog.h \
    ../src/gui/rb_databasetablewidget.h \
    ../src/model/EN1591/pcalc_en1591_assembly.h \
    ../src/model/EN1591/pcalc_en1591_boltnutwasher.h \
    ../src/model/EN1591/pcalc_en1591_flange.h \
    ../src/model/EN1591/pcalc_en1591_gasket.h \
    ../src/model/EN1591/pcalc_en1591_loadcase.h \
    ../src/model/EN1591/pcalc_en1591_shell.h \
    ../src/model/EN1591/std_en13555egeg.h \
    ../src/model/EN1591/std_en13555gasket.h \
    ../src/model/EN1591/std_en13555manufacturer.h \
    ../src/model/EN1591/std_en13555pqrdeltaegc.h \
    ../src/model/EN1591/std_en13555qsmaxalphag.h \
    ../src/model/EN1591/std_en13555qminl.h \
    ../src/model/EN1591/std_en13555qsminl.h \
    ../src/model/pcalc.h \
    ../src/model/pcalc_classgenerator.h \
    ../src/model/pcalc_generatorformula.h \
    ../src/model/pcalc_modelfactory.h \
    ../src/model/pcalc_objectfactory.h \
    ../src/utils/EN1591/en13445tableg.h \
    ../src/utils/EN1591/en13555property.h \
    ../src/utils/pcalc_report.h \
    ../src/utils/pcalc_sqlcommonfunctions.h \
    ../src/utils/pcalc_utilityfactory.h \
    ../src/utils/rb_tablemath.h


SOURCES += \
    ../../peng/src/model/peng_project.cpp \
    ../src/actions/pcalc_actionclose.cpp \
    ../src/actions/pcalc_actioneditproject.cpp \
    ../src/actions/pcalc_actionen1591flange.cpp \
    ../src/actions/pcalc_actionengasket.cpp \
    ../src/actions/pcalc_actionfactory.cpp \
    ../src/actions/pcalc_actionplugin.cpp \
    ../src/actions/pcalc_actionselectproject.cpp \
    ../src/calc/en1591handler.cpp \
    ../src/calc/EN1591/assembly.cpp \
    ../src/calc/EN1591/bolt.cpp \
    ../src/calc/EN1591/bolthole.cpp \
    ../src/calc/EN1591/calculator.cpp \
    ../src/calc/EN1591/flange.cpp \
    ../src/calc/EN1591/flange_blind.cpp \
    ../src/calc/EN1591/flange_integral.cpp \
    ../src/calc/EN1591/flange_loose.cpp \
    ../src/calc/EN1591/force.cpp \
    ../src/calc/EN1591/gasket.cpp \
    ../src/calc/EN1591/loadcase.cpp \
    ../src/calc/EN1591/loadcaselist.cpp \
    ../src/calc/EN1591/shell.cpp \
    ../src/calc/EN1591/washer.cpp \
    ../src/calc/EN1591/unittest/assemblytest.cpp \
    ../src/calc/EN1591/unittest/bolttest.cpp \
    ../src/calc/EN1591/unittest/flange_blindtest.cpp \
    ../src/calc/EN1591/unittest/flange_integraltest.cpp \
    ../src/calc/EN1591/unittest/flange_loosetest.cpp \
    ../src/calc/EN1591/unittest/flangetest.cpp \
    ../src/calc/EN1591/unittest/forcetest.cpp \
    ../src/calc/EN1591/unittest/gaskettest.cpp \
    ../src/calc/EN1591/unittest/washertest.cpp \
    ../src/calc/pcalc_inputoutput.cpp \
    ../src/calc/rb_allowanceservice.cpp \
    ../src/calc/rb_materialservice.cpp \
    ../src/calc/rb_qualityservice.cpp \
    ../src/calc/unittest/rb_tablemathtest.cpp \
    ../src/calc/unittest/rb_unittest.cpp \
    ../src/calc/unittest/rb_unittestfactory.cpp \
    ../src/calc/unittest/en1591_unittestfactory.cpp \
    ../src/gui/pcalc_classgeneratorwidget.cpp \
    ../src/gui/pcalc_dialogfactory.cpp \
    ../src/gui/pcalc_en13555gasketwidget.cpp \
    ../src/gui/pcalc_en13555manufwidget.cpp \
    ../src/gui/pcalc_en13555typewidget.cpp \
    ../src/gui/pcalc_en1591selectgasketwidget.cpp \
    ../src/gui/pcalc_en1591widget.cpp \
    ../src/gui/pcalc_projecteditdialog.cpp \
    ../src/gui/rb_databasetablewidget.cpp \
    ../src/model/EN1591/pcalc_en1591_assembly.cpp \
    ../src/model/EN1591/pcalc_en1591_boltnutwasher.cpp \
    ../src/model/EN1591/pcalc_en1591_flange.cpp \
    ../src/model/EN1591/pcalc_en1591_gasket.cpp \
    ../src/model/EN1591/pcalc_en1591_loadcase.cpp \
    ../src/model/EN1591/pcalc_en1591_shell.cpp \
    ../src/model/EN1591/std_en13555egeg.cpp \
    ../src/model/EN1591/std_en13555gasket.cpp \
    ../src/model/EN1591/std_en13555manufacturer.cpp \
    ../src/model/EN1591/std_en13555pqrdeltaegc.cpp \
    ../src/model/EN1591/std_en13555qminl.cpp \
    ../src/model/EN1591/std_en13555qsmaxalphag.cpp \
    ../src/model/EN1591/std_en13555qsminl.cpp \
    ../src/model/pcalc_classgenerator.cpp \
    ../src/model/pcalc_generatorformula.cpp \
    ../src/model/pcalc_modelfactory.cpp \
    ../src/model/pcalc_objectfactory.cpp \
    ../src/utils/EN1591/en13445tableg.cpp \
    ../src/utils/EN1591/en13555property.cpp \
    ../src/utils/pcalc_report.cpp \
    ../src/utils/pcalc_sqlcommonfunctions.cpp \
    ../src/utils/pcalc_utilityfactory.cpp \
    ../src/utils/rb_tablemath.cpp

RESOURCES += \
    ../../db/src/gui/db.qrc \
    ../src/gui/pcalc.qrc

OTHER_FILES += \
    ../src/actions/pcalc_actionplugin.json \
    ../src/gui/pcalc_en1591widget.dox
