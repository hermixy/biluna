TEMPLATE = app
CONFIG += warn_on link_prl release
INCLUDEPATH += $$(HOME)/local/include \
               ../include \
               ../../qcadactions/include \
               ../../qcadprof/include \
               ../../qcadlib/include \
               ../../dxflib/include \
               ../../fparser/include
LIBS += -L$$(HOME)/local/lib -lcppunit \
        -L../lib -lqcadguiqt \
        -L../../qcadprof/lib -lqcadprof \
        -L../../qcadactions/lib -lqcadactions \
        -L../../qcadlib/lib -lqcad \
        -L../../dxflib/lib -ldxf \
        -L../../fparser/lib -lfparser

SOURCES = main.cpp
HEADERS = \
          qg_blockwidgettest.h \
          qg_colorboxtest.h \
          qg_layerwidgettest.h \
          qg_graphicviewtest.h \
          qg_mainwindowinterfacetest.h

OBJECTS_DIR = obj
MOC_DIR     = moc


