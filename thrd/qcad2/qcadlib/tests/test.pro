TEMPLATE = app
#CONFIG = qt warn_on link_prl release console rs_prof rs_arch rs_algo
exists( ../../mkspecs/defs.pro ):include( ../../mkspecs/defs.pro )
INCLUDEPATH += \
    $$(HOME)/local/include \
    ../src/actions \
    ../src/creation \
    ../src/debug \
    ../src/entities \
    ../src/fileio \
    ../src/filters \
    ../src/global \
    ../src/gui \
    ../src/information \
    ../src/math \
    ../src/modification \
    ../src/scripting \
    ../src/tools \
    ../../dxflib/src \
    ../../fparser/src \
    ../../qcadalgo/src/triangulation
LIBS += -L$$(HOME)/local/lib -lcppunit \
        -L../lib -lqcad \
        -L../../qcadalgo/lib -lqcadalgo \
        -L../../dxflib/lib -llibdxf \
        -L../../fparser/lib -lfparser

SOURCES = main.cpp
HEADERS = \
          rs_arctest.h \
          rs_blocklisttest.h \
          rs_creationtest.h \
          rs_creationequidistanttest.h \
          rs_ellipsetest.h \
          rs_entitycontainertest.h \
          rs_extptrlisttest.h \
          rs_dimensiontest.h \
          rs_fonttest.h \
          rs_hatchtest.h \
          rs_infoareatest.h \
          rs_inserttest.h \
          rs_leadertest.h \
          rs_linetest.h \
          rs_mathtest.h \
          rs_matrixtest.h \
          rs_modificationdivide2test.h \
          rs_ptrlisttest.h \
          rs_test.h \
          rs_texttest.h \
          rs_undotest.h \
          rs_unitstest.h \
          rs_vectorlisttest.h \
          rs_vectortest.h

OBJECTS_DIR = .obj
MOC_DIR     = .moc

exists( ../../mkspecs/defs.pro ) {
    include( ../../mkspecs/defs.pro )
}
