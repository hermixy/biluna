#include <stdio.h>

#include <cppunit/ui/text/TestRunner.h>

#include <QApplication>

#include "rs_debug.h"

#include "qg_blockwidgettest.h"
#include "qg_layerwidgettest.h"
#include "qg_graphicviewtest.h"
#include "qg_colorboxtest.h"

int main(int argc, char** argv) {

    //RS_DEBUG->setLevel(RS_Debug::D_DEBUGGING);
    RS_DEBUG->setLevel(RS_Debug::D_TESTING);

    QApplication app(argc, argv);
    
    CppUnit::TextUi::TestRunner runner;
    runner.addTest( QG_BlockWidgetTest::suite() );
    runner.addTest( QG_LayerWidgetTest::suite() );
    runner.addTest( QG_GraphicViewTest::suite() );
    runner.addTest( QG_ColorBoxTest::suite() );
    bool ret = runner.run();

    //getchar();

    return ret;
}
