#include <stdio.h>
#include "rs_test.h"
#include "rs_actiondrawline.h"
#include "rs_bitmapgraphicview.h"

class RS_ActionDrawLineTest : public RS_Test {
    CPPUNIT_TEST_SUITE( RS_ActionDrawLineTest );
    CPPUNIT_TEST( testMove1 );
    CPPUNIT_TEST( testClick1 );
    CPPUNIT_TEST( testMove2 );
    CPPUNIT_TEST( testClick2 );
    CPPUNIT_TEST( testSequence );
    CPPUNIT_TEST( testClose );
    CPPUNIT_TEST( testTwoSequences );
    CPPUNIT_TEST( testTwoActions );
    CPPUNIT_TEST_SUITE_END();

private:
    RS_ActionDrawLine* action;
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_ActionDrawLineTest");
        RS_Test::setUp();
        
        action = new RS_ActionDrawLine(ec, *bitmapView);
        bitmapView->setCurrentAction(action);
    }

    void tearDown() {
        RS_Test::tearDown();
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_ActionDrawLineTest: OK");
    }

    void testMove1() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMove1");
        
        doMouseMove(15,30);

        bitmapView->save("data/output/line000_0.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/line000_0.png") ==
                       RS_Img("data/expected/line000_0.png"));
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMove1: OK");
    }

    void testClick1() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testClick1");

        doMouseMove(15,30);
        doMouseClick(15,30);
        
        bitmapView->save("data/output/line000_1.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/line000_1.png") ==
                       RS_Img("data/expected/line000_1.png"));

        RS_DEBUG->print(RS_Debug::D_TESTING, "testClick1: OK");
    }

    void testMove2() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMove2");
        
        doMouseMove(15,30);
        doMouseClick(15,30);
        doMouseMove(230,215);
        
        bitmapView->save("data/output/line000_2.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/line000_2.png") ==
                       RS_Img("data/expected/line000_2.png"));
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMove2: OK");
    }

    void testClick2() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testClick2");

        doMouseMove(15,30);
        doMouseClick(15,30);
        doMouseMove(230,215);
        doMouseClick(230,215);
        
        bitmapView->save("data/output/line000_3.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/line000_3.png") ==
                       RS_Img("data/expected/line000_3.png"));

        RS_Entity* e = ec.firstEntity();
        CPPUNIT_ASSERT( e!=NULL );
        CPPUNIT_ASSERT( e->rtti()==RS2::EntityLine );
        RS_Line* l = (RS_Line*)e;
        CPPUNIT_ASSERT( l->getStartpoint().distanceTo(RS_Vector(15,30)) 
            < RS_TOLERANCE );
        CPPUNIT_ASSERT( l->getEndpoint().distanceTo(RS_Vector(230,215)) 
            <  RS_TOLERANCE );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testClick2: OK");
    }
    
    void testSequence() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testSequence");

        for (double ang=0.0; ang<2*M_PI+1.0e-4; ang+=M_PI/8) {
            doMouseClick((int)(125+cos(ang)*110),
                         (int)(125+sin(ang)*110));
            doMouseClick((int)(125+cos(ang+M_PI/16)*25),
                         (int)(125+sin(ang+M_PI/16)*25));
        }
        
        bitmapView->save("data/output/line001.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/line001.png") ==
                       RS_Img("data/expected/line001.png"));

        RS_DEBUG->print(RS_Debug::D_TESTING, "testSequence: OK");
    }
    
    void testClose() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testClose");

        doMouseClick(10,10);
        doMouseClick(230,10);
        doMouseClick(125,230);
        action->close();
        
        bitmapView->save("data/output/line002.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/line002.png") ==
                       RS_Img("data/expected/line002.png"));
        
        RS_Entity* e;
        RS_Line* l;

        e = ec.firstEntity();
        CPPUNIT_ASSERT( e!=NULL );
        CPPUNIT_ASSERT( e->rtti()==RS2::EntityLine );
        l = (RS_Line*)e;
        CPPUNIT_ASSERT( l->getStartpoint().distanceTo(RS_Vector(10,10)) 
            < RS_TOLERANCE );
        CPPUNIT_ASSERT( l->getEndpoint().distanceTo(RS_Vector(230,10)) 
            <  RS_TOLERANCE );

        e = ec.nextEntity();
        CPPUNIT_ASSERT( e!=NULL );
        CPPUNIT_ASSERT( e->rtti()==RS2::EntityLine );
        l = (RS_Line*)e;
        CPPUNIT_ASSERT( l->getStartpoint().distanceTo(RS_Vector(230,10)) 
            < RS_TOLERANCE );
        CPPUNIT_ASSERT( l->getEndpoint().distanceTo(RS_Vector(125,230)) 
            <  RS_TOLERANCE );
        
        e = ec.nextEntity();
        CPPUNIT_ASSERT( e!=NULL );
        CPPUNIT_ASSERT( e->rtti()==RS2::EntityLine );
        l = (RS_Line*)e;
        CPPUNIT_ASSERT( l->getStartpoint().distanceTo(RS_Vector(125,230)) 
            < RS_TOLERANCE );
        CPPUNIT_ASSERT( l->getEndpoint().distanceTo(RS_Vector(10,10)) 
            <  RS_TOLERANCE );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testClose: OK");
    }
    
    void testTwoSequences() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testTwoSequences");

        doMouseClick(10,10);
        doMouseClick(200,30);
        doMouseClick(10,50);
        
        doRightClick();
        
        doMouseClick(10,70);
        doMouseClick(200,90);
        doMouseClick(10,110);
        
        bitmapView->save("data/output/line003.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/line003.png") ==
                       RS_Img("data/expected/line003.png"));

        RS_DEBUG->print(RS_Debug::D_TESTING, "testTwoSequences: OK");
    }
    
    void testTwoActions() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testTwoActions");

        doMouseClick(10,10);
        doMouseClick(200,30);
        doMouseClick(10,50);
        
        doRightClick();
        doRightClick();

        action = new RS_ActionDrawLine(ec, *bitmapView);
        bitmapView->setCurrentAction(action);
        
        doMouseClick(10,70);
        doMouseClick(200,90);
        doMouseClick(10,110);
        
        bitmapView->save("data/output/line004.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/line004.png") ==
                       RS_Img("data/expected/line004.png"));

        RS_DEBUG->print(RS_Debug::D_TESTING, "testTwoActions: OK");
    }

protected:
    RS_EntityContainer ec;
};
