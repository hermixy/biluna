TEMPLATE = app
CONFIG += warn_on link_prl release
INCLUDEPATH += $$(HOME)/local/include \
               ../include \
               ../../qcadlib/include \
               ../../dxflib/include \
               ../../fparser/include
LIBS += -L$$(HOME)/local/lib -lcppunit \
        -L../lib -lqcadactions \
        -L../../qcadlib/lib -lqcad \
        -L../../dxflib/lib -ldxf \
        -L../../fparser/lib -lfparser

SOURCES = main.cpp
HEADERS = \
          rs_actionblocksinserttest.h \
          rs_actiondefaulttest.h \
          rs_actiondrawlinetest.h \
          rs_actiondimleadertest.h \
          rs_actioninfodist2test.h \
          rs_test.h

OBJECTS_DIR = obj
MOC_DIR     = moc

