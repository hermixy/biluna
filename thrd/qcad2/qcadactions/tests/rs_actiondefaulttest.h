#include <stdio.h>
#include "rs_test.h"
#include "rs_line.h"
#include "rs_actiondefault.h"
#include "rs_bitmapgraphicview.h"

class RS_ActionDefaultTest : public RS_Test {
    CPPUNIT_TEST_SUITE( RS_ActionDefaultTest );
    CPPUNIT_TEST( testDragNDrop );
    CPPUNIT_TEST_SUITE_END();

private:
    RS_ActionDefault* action;
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_ActionDefaultTest");
        RS_Test::setUp();

        action = new RS_ActionDefault(ec, *bitmapView);
        bitmapView->setCurrentAction(action);
    }

    void tearDown() {
        RS_Test::tearDown();
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_ActionDefaultTest: OK");
    }

    void testDragNDrop() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDragNDrop");
        
        RS_Line* line = new RS_Line(&ec, 
            RS_LineData(RS_Vector(0,0), RS_Vector(100,100)));
        ec.addEntity(line);
        
        CPPUNIT_ASSERT(ec.countSelected()==0);

        bitmapView->save("data/output/default000_0.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/default000_0.png") ==
                       RS_Img("data/expected/default000_0.png"));

        doMouseClick(25,25);
        
        CPPUNIT_ASSERT(ec.countSelected()==1);
        
        bitmapView->save("data/output/default000_1.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/default000_1.png") ==
                       RS_Img("data/expected/default000_1.png"));
        
        bitmapView->clear();

        doMousePress(25,25);
        doMouseMove(50,10);
        doMouseRelease(50,10);
        doMouseClick(50,10);
        
        bitmapView->save("data/output/default000_2.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/default000_2.png") ==
                       RS_Img("data/expected/default000_2.png"));
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDragNDrop: OK");
    }

protected:
    RS_EntityContainer ec;
};
