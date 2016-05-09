#-------------------------------------------------
# $Id: acc.pro 1267 2010-08-02 16:14:33Z rutger $
# Project created by QtCreator 2009-12-18T18:11:30
# - for building accounting application ACC
#-------------------------------------------------

TEMPLATE    = app
LANGUAGE    = C++
TARGET      = Biluna

# icon
RC_FILE     = ../../db/src/gui/images/app_ico.rc

# compiler settings for release, debug and some paths
exists( ../../mkspecs/defs.pro ) {
    include( ../../mkspecs/defs.pro )
} else {
    # message and exit
    error( "An error has occurred, ../../mkspecs/defs.pro is missing." )
}

INCLUDEPATH += \
    ../../db/src/actions \
    ../../db/src/entities \
    ../../db/src/gui \
    ../../db/src/gui/ui \
    ../../db/src/model \
    ../../db/src/operations \
    ../../db/src/utils \
    ../../db/src/visitors \
    ../../thrd/qtsingleapplication/src \
    actions \
    filters \
    gui \
    model \
    operations \
    utilities



# Carefull with the sequence of the libraries, most basic one last
CONFIG(debug, debug|release) {
    message("Debug libraries used for bil")
    LIBS	+= \
        -L../../bil/debug -ldbd
} else {
    message("Release libraries used for bil")
    LIBS	+= \
        -L../../bil/release -ldb
}

# add forms, headers and sources
exists( ../src/bil.pri ) {
    include( ../src/bil.pri )
} else {
    # message and exit
    error( "An error has occurred, ../src/bil.pri is missing." )
}

exists( ../../thrd/qtsingleapplication/src/qtsingleapplication.pri ) {
    include( ../../thrd/qtsingleapplication/src/qtsingleapplication.pri )
} else {
    # message and exit
    error( "An error has occurred, ../../thrd/qtsingleapplication/src/qtsingleapplication.pri is missing." )
}
