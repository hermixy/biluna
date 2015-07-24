INCLUDEPATH += \
    $$PWD \
    $$PWD/animations \
    $$PWD/areachart \
    $$PWD/axis \
    $$PWD/barchart \
    $$PWD/boxplotchart \
    $$PWD/domain \
    $$PWD/layout \
    $$PWD/legend \
    $$PWD/linechart \
    $$PWD/piechart \
    $$PWD/scatterchart \
    $$PWD/splinechart \
    $$PWD/themes \
    $$PWD/xychart

PRIVATE_HEADERS += \
    $$PWD/chartbackground_p.h \
    $$PWD/chartconfig_p.h \
    $$PWD/chartdataset_p.h \
    $$PWD/chartelement_p.h \
    $$PWD/charthelpers_p.h \
    $$PWD/chartitem_p.h \
    $$PWD/chartpresenter_p.h \
    $$PWD/chartthememanager_p.h \
    $$PWD/charttitle_p.h \
    $$PWD/qabstractseries_p.h \
    $$PWD/qchart_p.h \
    $$PWD/qchartview_p.h \
    $$PWD/scroller_p.h

PUBLIC_HEADERS += \
    $$PWD/chartsnamespace.h \
    $$PWD/qabstractseries.h \
    $$PWD/qchart.h \
    $$PWD/qchartglobal.h \
    $$PWD/qchartview.h \
    $$PWD/qpolarchart.h \

SOURCES += \
    $$PWD/chartbackground.cpp \
    $$PWD/chartdataset.cpp \
    $$PWD/chartelement.cpp \
    $$PWD/chartitem.cpp \
    $$PWD/chartpresenter.cpp \
    $$PWD/chartthememanager.cpp \
    $$PWD/charttitle.cpp \
    $$PWD/qabstractseries.cpp \
    $$PWD/qchart.cpp \
    $$PWD/qchartview.cpp \
    $$PWD/qpolarchart.cpp \
    $$PWD/scroller.cpp

include(animations/animations.pri)
include(areachart/areachart.pri)
include(axis/axis.pri)
include(domain/domain.pri)
include(barchart/barchart.pri)
include(legend/legend.pri)
include(linechart/linechart.pri)
include(piechart/piechart.pri)
include(scatterchart/scatter.pri)
include(splinechart/splinechart.pri)
include(themes/themes.pri)
include(xychart/xychart.pri)
include(layout/layout.pri)
include(boxplotchart/boxplotchart.pri)

HEADERS += $$PUBLIC_HEADERS
HEADERS += $$PRIVATE_HEADERS
HEADERS += $$THEMES
