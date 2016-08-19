#include <stdio.h>

#include <cppunit/ui/text/TestRunner.h>

#include <QApplication>

#include "rs_debug.h"
#include "rs_fileio.h"
#include "rs_filtercxf.h"
#include "rs_filterdxf.h"
#include "rs_fontlist.h"
#include "rs_system.h"

#include "rs_arctest.h"
#include "rs_blocklisttest.h"
#include "rs_creationtest.h"
#include "rs_creationequidistanttest.h"
#include "rs_ellipsetest.h"
#include "rs_entitycontainertest.h"
#include "rs_extptrlisttest.h"
#include "rs_dimensiontest.h"
#include "rs_fonttest.h"
#include "rs_hatchtest.h"
#include "rs_infoareatest.h"
#include "rs_inserttest.h"
#include "rs_leadertest.h"
#include "rs_linetest.h"
#include "rs_mathtest.h"
#include "rs_matrixtest.h"
#include "rs_modificationdivide2test.h"
#include "rs_ptrlisttest.h"
#include "rs_texttest.h"
#include "rs_undotest.h"
#include "rs_unitstest.h"
#include "rs_vectorlisttest.h"
#include "rs_vectortest.h"

int main(int argc, char** argv) {

    RS_DEBUG->setLevel(RS_Debug::D_DEBUGGING);
    //RS_DEBUG->setLevel(RS_Debug::D_TESTING);

    RS_SYSTEM->init("qcadlibtest", "0", "qcadlibtest", 
        QFileInfo(QFile::decodeName(argv[0])).absolutePath());
    RS_FILEIO->registerFilter(new RS_FilterDxf());
    RS_FILEIO->registerFilter(new RS_FilterCxf());

    QApplication app(argc, argv);
    
    CppUnit::TextUi::TestRunner runner;
    
    runner.addTest( RS_ArcTest::suite() );
    runner.addTest( RS_BlockListTest::suite() );
    runner.addTest( RS_CreationTest::suite() );
    runner.addTest( RS_CreationEquidistantTest::suite() );
    runner.addTest( RS_EllipseTest::suite() );
    runner.addTest( RS_EntityContainerTest::suite() );
    runner.addTest( RS_ExtPtrListTest::suite() );
    runner.addTest( RS_DimensionTest::suite() );
    runner.addTest( RS_FontTest::suite() );
    runner.addTest( RS_HatchTest::suite() );
    runner.addTest( RS_InfoAreaTest::suite() );
    runner.addTest( RS_InsertTest::suite() );
    runner.addTest( RS_LeaderTest::suite() );
    runner.addTest( RS_LineTest::suite() );
    runner.addTest( RS_MathTest::suite() );
    runner.addTest( RS_MatrixTest::suite() );
    runner.addTest( RS_ModificationDivide2Test::suite() );
    runner.addTest( RS_PtrListTest::suite() );
    runner.addTest( RS_TextTest::suite() );
    runner.addTest( RS_UndoTest::suite() );
    runner.addTest( RS_UnitsTest::suite() );
    runner.addTest( RS_VectorListTest::suite() );
    runner.addTest( RS_VectorTest::suite() );
    
    bool ret = runner.run();

    return ret;
}
