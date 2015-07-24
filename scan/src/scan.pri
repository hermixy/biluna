#-------------------------------------------------
# $Id: acc.pri 1352 2011-01-26 22:01:25Z rutger $
# Project created 2011-04-17
# - for building scan or interviews SCAN plugin
# - ../src path is required to use .pri file also from ../test
# ----------------------------------------------------------

FORMS += \
    ../src/gui/scan_scaneditdialog.ui \
    ../src/gui/scan_scanresultwidget.ui

HEADERS += \
    ../src/actions/scan_actionclose.h \
    ../src/actions/scan_actioneditproject.h \
    ../src/actions/scan_actioneditscan.h \
    ../src/actions/scan_actionfactory.h \
    ../src/actions/scan_actionplugin.h \
    ../src/actions/scan_actionresult.h \
    ../src/actions/scan_actionreviewchart.h \
    ../src/actions/scan_actionselectproject.h \
    ../src/gui/scan_chartwidget.h \
    ../src/gui/scan_dialogfactory.h \
    ../src/gui/scan_ledelegate.h \
    ../src/gui/scan_projectdialog.h \
    ../src/gui/scan_projecteditdialog.h \
    ../src/gui/scan_scaneditdialog.h \
    ../src/gui/scan_scanselectdialog.h \
    ../src/gui/scan_scanresultwidget.h \
    ../src/model/scan.h \
    ../src/model/scan_answer.h \
    ../src/model/scan_answerresult.h \
    ../src/model/scan_block.h \
#    ../src/model/scan_chapter.h \
#    ../src/model/scan_graph.h \
    ../src/model/scan_modelfactory.h \
    ../src/model/scan_objectfactory.h \
#    ../src/model/scan_paragraph.h \
    ../src/model/scan_project.h \
    ../src/model/scan_question.h \
    ../src/model/scan_questionresult.h \
#    ../src/model/scan_report.h \
    ../src/model/scan_scan.h \
    ../src/model/scan_scanresult.h \
    ../src/operations/scan_preparejrreport.h \
    ../src/operations/scan_preparejrresult.h

SOURCES += \
    ../src/actions/scan_actionclose.cpp \
    ../src/actions/scan_actioneditproject.cpp \
    ../src/actions/scan_actioneditscan.cpp \
    ../src/actions/scan_actionfactory.cpp \
    ../src/actions/scan_actionplugin.cpp \
    ../src/actions/scan_actionresult.cpp \
    ../src/actions/scan_actionreviewchart.cpp \
    ../src/actions/scan_actionselectproject.cpp \
    ../src/gui/scan_chartwidget.cpp \
    ../src/gui/scan_dialogfactory.cpp \
    ../src/gui/scan_ledelegate.cpp \
    ../src/gui/scan_projecteditdialog.cpp \
    ../src/gui/scan_scaneditdialog.cpp \
    ../src/gui/scan_scanresultwidget.cpp \
    ../src/model/scan_answer.cpp \
    ../src/model/scan_answerresult.cpp \
    ../src/model/scan_block.cpp \
#    ../src/model/scan_chapter.cpp \
#    ../src/model/scan_graph.cpp \
    ../src/model/scan_modelfactory.cpp \
    ../src/model/scan_objectfactory.cpp \
#    ../src/model/scan_paragraph.cpp \
    ../src/model/scan_project.cpp \
    ../src/model/scan_question.cpp \
    ../src/model/scan_questionresult.cpp \
#    ../src/model/scan_report.cpp \
    ../src/model/scan_scan.cpp \
    ../src/model/scan_scanresult.cpp \
    ../src/operations/scan_preparejrreport.cpp \
    ../src/operations/scan_preparejrresult.cpp

RESOURCES += \
#    ../src/gui/scan.qrc \
    ../../db/src/gui/db.qrc

OTHER_FILES += \
    ../src/actions/scan_actionplugin.json \
    ../src/gui/scan_index.dox \
    ../src/gui/scan_projecteditdialog.dox \
    ../src/gui/scan_scaneditdialog.dox \
    ../src/gui/scan_scanresultwidget.dox
