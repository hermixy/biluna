#include <stdio.h>
#include <cppunit/extensions/HelperMacros.h>
#include "rs_layerlist.h"
#include "rs_graphic.h"
#include "qg_graphicview.h"
#include "rs_actiondefault.h"

class QG_GraphicViewTest : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( QG_GraphicViewTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_GraphicViewTest");
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_GraphicViewTest: OK");
    }
    
    void test() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "test");

        // init:
        QG_GraphicView* graphicView = new QG_GraphicView();
        RS_Graphic* graphic = new RS_Graphic();
        RS_Line* line = 
            new RS_Line(graphic, RS_LineData(RS_Vector(0,0), RS_Vector(100,100)));\
        line->setPen(RS_Pen(RS_Color(255,0,0), RS2::Width17, RS2::SolidLine));
        graphic->addEntity(line);
        graphicView->setContainer(graphic);
        graphicView->setDefaultAction(new RS_ActionDefault(*graphic, *graphicView));
        
        graphicView->show();
        qApp->exec();

        delete graphicView;
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "test: OK");
    }
};
