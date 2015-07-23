#
# Browser
#

FORMS += \
    ../../thrd/browser/addbookmarkdialog.ui \
    ../../thrd/browser/bookmarks.ui \
    ../../thrd/browser/cookies.ui \
    ../../thrd/browser/cookiesexceptions.ui \
    ../../thrd/browser/downloaditem.ui \
    ../../thrd/browser/downloads.ui \
    ../../thrd/browser/history.ui \
    ../../thrd/browser/passworddialog.ui \
    ../../thrd/browser/proxy.ui \
    ../../thrd/browser/settings.ui

HEADERS += \
    ../../thrd/browser/autosaver.h \
    ../../thrd/browser/bookmarks.h \
#     ../../thrd/browser/db_internetbrowserfactory.h \
#     ../../thrd/browser/browsermainwindow.h \
    ../../thrd/browser/chasewidget.h \
    ../../thrd/browser/cookiejar.h \
    ../../thrd/browser/downloadmanager.h \
    ../../thrd/browser/edittableview.h \
    ../../thrd/browser/edittreeview.h \
    ../../thrd/browser/history.h \
    ../../thrd/browser/modelmenu.h \
    ../../thrd/browser/networkaccessmanager.h \
    ../../thrd/browser/searchlineedit.h \
    ../../thrd/browser/settings.h \
    ../../thrd/browser/squeezelabel.h \
    ../../thrd/browser/tabwidget.h \
    ../../thrd/browser/toolbarsearch.h \
    ../../thrd/browser/urllineedit.h \
    ../../thrd/browser/webview.h \
    ../../thrd/browser/xbel.h

SOURCES += \
    ../../thrd/browser/autosaver.cpp \
    ../../thrd/browser/bookmarks.cpp \
#     ../../thrd/browser/browserapplication.cpp \
#     ../../thrd/browser/browsermainwindow.cpp \
    ../../thrd/browser/chasewidget.cpp \
    ../../thrd/browser/cookiejar.cpp \
    ../../thrd/browser/downloadmanager.cpp \
    ../../thrd/browser/edittableview.cpp \
    ../../thrd/browser/edittreeview.cpp \
    ../../thrd/browser/history.cpp \
    ../../thrd/browser/modelmenu.cpp \
    ../../thrd/browser/networkaccessmanager.cpp \
    ../../thrd/browser/searchlineedit.cpp \
    ../../thrd/browser/settings.cpp \
    ../../thrd/browser/squeezelabel.cpp \
    ../../thrd/browser/tabwidget.cpp \
    ../../thrd/browser/toolbarsearch.cpp \
    ../../thrd/browser/urllineedit.cpp \
    ../../thrd/browser/webview.cpp \
    ../../thrd/browser/xbel.cpp
#     ../../thrd/browser/main.cpp

RESOURCES += \
    ../../thrd/browser/data/data.qrc \
    ../../thrd/browser/htmls/htmls.qrc

#
# Nightcharts
#
HEADERS += \
    ../../thrd/nightcharts/thrd_chart.h \
    ../../thrd/nightcharts/thrd_chartwidget.h

SOURCES += \
    ../../thrd/nightcharts/thrd_chart.cpp \
    ../../thrd/nightcharts/thrd_chartwidget.cpp

# RESOURCES += \

FORMS += \
    ../../thrd/nightcharts/thrd_chartwidget.ui

#
# QHexedit2 hexadecimal viewer/editor for binary files
#
HEADERS += \
    ../../thrd/qhexedit2/src/chunks.h \
    ../../thrd/qhexedit2/src/commands.h \
    ../../thrd/qhexedit2/src/qhexedit.h

SOURCES += \
    ../../thrd/qhexedit2/src/chunks.cpp \
    ../../thrd/qhexedit2/src/commands.cpp \
    ../../thrd/qhexedit2/src/qhexedit.cpp

#
# Speedcruch calculator
#
HEADERS += \
    ../../thrd/speedcrunch-0.10.1/src/3rdparty/util/binreloc.h \
    ../../thrd/speedcrunch-0.10.1/src/base/constants.hxx \
    ../../thrd/speedcrunch-0.10.1/src/base/errors.h \
    ../../thrd/speedcrunch-0.10.1/src/base/evaluator.hxx \
    ../../thrd/speedcrunch-0.10.1/src/base/functions.hxx \
#    ../../thrd/speedcrunch-0.10.1/src/base/settings.hxx \
    ../../thrd/speedcrunch-0.10.1/src/gui/aboutbox.hxx \
#    ../../thrd/speedcrunch-0.10.1/src/gui/application.hxx \
    ../../thrd/speedcrunch-0.10.1/src/gui/autohidelabel.hxx \
    ../../thrd/speedcrunch-0.10.1/src/gui/bookdock.hxx \
    ../../thrd/speedcrunch-0.10.1/src/gui/constantsdock.hxx \
#    ../../thrd/speedcrunch-0.10.1/src/gui/constantswidget.hxx \
    ../../thrd/speedcrunch-0.10.1/src/gui/deletevardlg.hxx \
    ../../thrd/speedcrunch-0.10.1/src/gui/editor.hxx \
#    ../../thrd/speedcrunch-0.10.1/src/gui/functionsdialog.hxx \
    ../../thrd/speedcrunch-0.10.1/src/gui/functionsdock.hxx \
#    ../../thrd/speedcrunch-0.10.1/src/gui/functionswidget.hxx \
    ../../thrd/speedcrunch-0.10.1/src/gui/historydock.hxx \
#    ../../thrd/speedcrunch-0.10.1/src/gui/historywidget.hxx \
    ../../thrd/speedcrunch-0.10.1/src/gui/insertfunctiondlg.hxx \
    ../../thrd/speedcrunch-0.10.1/src/gui/insertvardlg.hxx \
#    ../../thrd/speedcrunch-0.10.1/src/gui/keypad.hxx \
    ../../thrd/speedcrunch-0.10.1/src/gui/resultdisplay.hxx \
    #../../thrd/speedcrunch-0.10.1/src/gui/syntaxhighlighter.hxx \
    #../../thrd/speedcrunch-0.10.1/src/gui/textedit.hxx \
    ../../thrd/speedcrunch-0.10.1/src/gui/tipwidget.hxx \
    ../../thrd/speedcrunch-0.10.1/src/gui/variablesdock.hxx \
#    ../../thrd/speedcrunch-0.10.1/src/gui/variableswidget.hxx \
#    ../../thrd/speedcrunch-0.10.1/src/gui/mainwindow.hxx
    #../../thrd/speedcrunch-0.10.1/src/math/floatcommon.h \
    #../../thrd/speedcrunch-0.10.1/src/math/floatconfig.h \
    #../../thrd/speedcrunch-0.10.1/src/math/floatconst.h \
    #../../thrd/speedcrunch-0.10.1/src/math/floatconvert.h \
    #../../thrd/speedcrunch-0.10.1/src/math/floaterf.h \
    #../../thrd/speedcrunch-0.10.1/src/math/floatexp.h \
    #../../thrd/speedcrunch-0.10.1/src/math/floatgamma.h \
    #../../thrd/speedcrunch-0.10.1/src/math/floathmath.h \
    #../../thrd/speedcrunch-0.10.1/src/math/floatio.h \
    #../../thrd/speedcrunch-0.10.1/src/math/floatipower.h \
    #../../thrd/speedcrunch-0.10.1/src/math/floatlog.h \
    #../../thrd/speedcrunch-0.10.1/src/math/floatlogic.h \
    #../../thrd/speedcrunch-0.10.1/src/math/floatlong.h \
    #../../thrd/speedcrunch-0.10.1/src/math/floatnum.h \
    #../../thrd/speedcrunch-0.10.1/src/math/floatpower.h \
    #../../thrd/speedcrunch-0.10.1/src/math/floatseries.h \
    #../../thrd/speedcrunch-0.10.1/src/math/floattrig.h \
    #../../thrd/speedcrunch-0.10.1/src/math/hmath.hxx \
    #../../thrd/speedcrunch-0.10.1/src/math/number.h

#YACCSOURCES += ../../thrd/speedcrunch-0.10.1/src/bison/exprparser.y

SOURCES += \
    ../../thrd/speedcrunch-0.10.1/src/3rdparty/util/binreloc.c \
    #../../thrd/speedcrunch-0.10.1/src/variant/variantbase.cpp \
    #../../thrd/speedcrunch-0.10.1/src/variant/variant.cpp \
    #../../thrd/speedcrunch-0.10.1/src/variant/real.cpp \
    #../../thrd/speedcrunch-0.10.1/src/variant/vstring.cpp \
    #../../thrd/speedcrunch-0.10.1/src/variant/initvariant.cpp \
    #../../thrd/speedcrunch-0.10.1/src/variant/formatbase.cpp \
    #../../thrd/speedcrunch-0.10.1/src/variant/formatreal.cpp \
    ../../thrd/speedcrunch-0.10.1/src/base/constants.cpp \
    ../../thrd/speedcrunch-0.10.1/src/base/evaluator.cpp \
    ../../thrd/speedcrunch-0.10.1/src/base/functions.cpp \
#    ../../thrd/speedcrunch-0.10.1/src/base/settings.cpp \
    ../../thrd/speedcrunch-0.10.1/src/gui/aboutbox.cpp \
#    ../../thrd/speedcrunch-0.10.1/src/gui/application.cpp \
    ../../thrd/speedcrunch-0.10.1/src/gui/autohidelabel.cpp \
    ../../thrd/speedcrunch-0.10.1/src/gui/bookdock.cpp \
    ../../thrd/speedcrunch-0.10.1/src/gui/constantsdock.cpp \
#    ../../thrd/speedcrunch-0.10.1/src/gui/constantswidget.cpp \
    ../../thrd/speedcrunch-0.10.1/src/gui/deletevardlg.cpp \
#    ../../thrd/speedcrunch-0.10.1/src/gui/resultdisplay.cpp \
    ../../thrd/speedcrunch-0.10.1/src/gui/editor.cpp \
#    ../../thrd/speedcrunch-0.10.1/src/gui/functionsdialog.cpp \
    ../../thrd/speedcrunch-0.10.1/src/gui/functionsdock.cpp \
#    ../../thrd/speedcrunch-0.10.1/src/gui/functionswidget.cpp \
    ../../thrd/speedcrunch-0.10.1/src/gui/historydock.cpp \
#    ../../thrd/speedcrunch-0.10.1/src/gui/historywidget.cpp \
    ../../thrd/speedcrunch-0.10.1/src/gui/insertfunctiondlg.cpp \
    ../../thrd/speedcrunch-0.10.1/src/gui/insertvardlg.cpp \
#    ../../thrd/speedcrunch-0.10.1/src/gui/keypad.cpp \
    ../../thrd/speedcrunch-0.10.1/src/gui/resultdisplay.cpp \
#    ../../thrd/speedcrunch-0.10.1/src/gui/syntaxhighlighter.cpp \
#    ../../thrd/speedcrunch-0.10.1/src/gui/textedit.cpp \
    ../../thrd/speedcrunch-0.10.1/src/gui/tipwidget.cpp \
    ../../thrd/speedcrunch-0.10.1/src/gui/variablesdock.cpp \
#    ../../thrd/speedcrunch-0.10.1/src/gui/variableswidget.cpp \
#    ../../thrd/speedcrunch-0.10.1/src/gui/mainwindow.cpp \
#    ../../thrd/speedcrunch-0.10.1/src/main/main.cpp \
    ../../thrd/speedcrunch-0.10.1/src/math/floatcommon.c \
    ../../thrd/speedcrunch-0.10.1/src/math/floatconst.c \
    ../../thrd/speedcrunch-0.10.1/src/math/floatconvert.c \
    ../../thrd/speedcrunch-0.10.1/src/math/floaterf.c \
    ../../thrd/speedcrunch-0.10.1/src/math/floatexp.c \
    ../../thrd/speedcrunch-0.10.1/src/math/floatgamma.c \
    ../../thrd/speedcrunch-0.10.1/src/math/floathmath.c \
    ../../thrd/speedcrunch-0.10.1/src/math/floatio.c \
    ../../thrd/speedcrunch-0.10.1/src/math/floatipower.c \
    ../../thrd/speedcrunch-0.10.1/src/math/floatlog.c \
    ../../thrd/speedcrunch-0.10.1/src/math/floatlogic.c \
    ../../thrd/speedcrunch-0.10.1/src/math/floatlong.c \
    ../../thrd/speedcrunch-0.10.1/src/math/floatnum.c \
    ../../thrd/speedcrunch-0.10.1/src/math/floatpower.c \
    ../../thrd/speedcrunch-0.10.1/src/math/floatseries.c \
    ../../thrd/speedcrunch-0.10.1/src/math/floattrig.c \
    ../../thrd/speedcrunch-0.10.1/src/math/hmath.cpp \
    ../../thrd/speedcrunch-0.10.1/src/math/number.c

!macx {
    !win32 {
        SOURCES += 3rdparty/binreloc/binreloc.c
    }
}

RESOURCES += ../../thrd/speedcrunch-0.10.1/src/resources/speedcrunch.qrc
# win32:RC_FILE = ../../thrd/speedcrunch-0.10.1/src/resources/speedcrunchico.rc

