#include <stdio.h>

#include <cppunit/ui/text/TestRunner.h>

#include <QApplication>

#include "rs_debug.h"
#include "rs_fontlist.h"
#include "rs_system.h"

#include "rs_actionblocksinserttest.h"
#include "rs_actiondefaulttest.h"
#include "rs_actiondrawlinetest.h"
#include "rs_actiondimleadertest.h"
#include "rs_actioninfodist2test.h"

int main(int argc, char** argv) {

    RS_DEBUG->setLevel(RS_Debug::D_DEBUGGING);
    //RS_DEBUG->setLevel(RS_Debug::D_TESTING);

    QFileInfo prgInfo( QFile::decodeName(argv[0]) );
    QString prgDir(prgInfo.absolutePath());

    //RS_FILEIO->registerFilter(new RS_FilterDXF());
    RS_SYSTEM->init("qcadactions_test", "0", "qcad", "");
    RS_FONTLIST->init();

    QApplication app(argc, argv);
    
    CppUnit::TextUi::TestRunner runner;

    /*
    runner.addTest( RS_ActionBlocksInsertTest::suite() );
    runner.addTest( RS_ActionDefaultTest::suite() );
    runner.addTest( RS_ActionDrawLineTest::suite() );
    runner.addTest( RS_ActionDimLeaderTest::suite() );
    runner.addTest( RS_ActionInfoDist2Test::suite() );
    */
    
    runner.addTest( RS_ActionDefaultTest::suite() );
    bool ret = runner.run();

    //getchar();

    return ret;
}
