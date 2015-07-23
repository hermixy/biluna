#-------------------------------------------------
# $Id: nightcharts.pri 1435 2011-07-14 17:15:58Z rutger $
# - for testing third-party nightcharts
# ----------------------------------------------------------

# FORMS += \

# ../nightcharts path from the perspective of thrd_test.pro
HEADERS += \
    ../../thrd/nightcharts/thrd_chart.h \
    ../../thrd/nightcharts/thrd_chartwidget.h

SOURCES += \
    ../../thrd/nightcharts/thrd_chart.cpp \
    ../../thrd/nightcharts/thrd_chartwidget.cpp

# RESOURCES += \

FORMS += \
    ../../thrd/nightcharts/thrd_chartwidget.ui
