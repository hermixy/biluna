#include <stdio.h>
#include "rs_test.h"
#include "rs_line.h"
#include "rs_actioninfodist2.h"
#include "rs_bitmapgraphicview.h"

class RS_ActionInfoDist2Test : public RS_Test {
    CPPUNIT_TEST_SUITE( RS_ActionInfoDist2Test );
    CPPUNIT_TEST( testMove1 );
    CPPUNIT_TEST( testClick1 );
    CPPUNIT_TEST( testMove2 );
    CPPUNIT_TEST( testClick2 );
    CPPUNIT_TEST_SUITE_END();

private:
    RS_ActionInfoDist2* action;
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_ActionInfoDist2Test");
        RS_Test::setUp();

        RS_Line* line = new RS_Line(&ec, 
            RS_LineData(RS_Vector(10,0), RS_Vector(10,250)));
        ec.addEntity(line);
        
        action = new RS_ActionInfoDist2(ec, *bitmapView);
        bitmapView->setCurrentAction(action);
    }

    void tearDown() {
        RS_Test::tearDown();
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_ActionInfoDist2Test: OK");
    }

    void testMove1() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMove1");
        
        doMouseMove(15,30);

        bitmapView->save("data/output/infodist2000_0.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/infodist2000_0.png") ==
                       RS_Img("data/expected/infodist2000_0.png"));
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMove1: OK");
    }

    void testClick1() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testClick1");

        doMouseMove(15,30);
        doMouseClick(15,30);
        
        bitmapView->save("data/output/infodist2000_1.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/infodist2000_1.png") ==
                       RS_Img("data/expected/infodist2000_1.png"));

        RS_DEBUG->print(RS_Debug::D_TESTING, "testClick1: OK");
    }

    void testMove2() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMove2");
        
        doMouseMove(15,30);
        doMouseClick(15,30);
        doMouseMove(200,215);
        
        bitmapView->save("data/output/infodist2000_2.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/infodist2000_2.png") ==
                       RS_Img("data/expected/infodist2000_2.png"));
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMove2: OK");
    }

    void testClick2() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testClick2");

        doMouseMove(15,30);
        doMouseClick(15,30);
        doMouseMove(200,215);
        doMouseClick(200,215);
        
        bitmapView->save("data/output/infodist2000_3.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/infodist2000_3.png") ==
                       RS_Img("data/expected/infodist2000_3.png"));

        RS_DEBUG->print(RS_Debug::D_TESTING, "testClick2: OK");
    }
    
protected:
    RS_EntityContainer ec;
};
