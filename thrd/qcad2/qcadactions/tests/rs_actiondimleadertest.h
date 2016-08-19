#include <stdio.h>
#include "rs_test.h"
#include "rs_actiondimleader.h"
#include "rs_bitmapgraphicview.h"
#include "rs_graphic.h"

class RS_ActionDimLeaderTest : public RS_Test {
    CPPUNIT_TEST_SUITE( RS_ActionDimLeaderTest );
    CPPUNIT_TEST( testMove1 );
    CPPUNIT_TEST( testClick1 );
    CPPUNIT_TEST( testMove2 );
    CPPUNIT_TEST( testClick2 );
    CPPUNIT_TEST( testSequence );
    CPPUNIT_TEST( testTwoSequences );
    CPPUNIT_TEST( testTwoActions );
    CPPUNIT_TEST( testUndo );
    CPPUNIT_TEST_SUITE_END();

private:
    RS_ActionDimLeader* action;
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_ActionDimLeaderTest");
        RS_Test::setUp();
        
        ec.addVariable("$DIMASZ", 15.0, 40);
        action = new RS_ActionDimLeader(ec, *bitmapView);
        bitmapView->setCurrentAction(action);
    }

    void tearDown() {
        RS_Test::tearDown();
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_ActionDimLeaderTest: OK");
    }

    void testMove1() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMove1");
        
        doMouseMove(15,30);

        bitmapView->save("data/output/leader000_0.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/leader000_0.png") ==
                       RS_Img("data/expected/leader000_0.png"));
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMove1: OK");
    }

    void testClick1() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testClick1");

        doMouseMove(15,30);
        doMouseClick(15,30);
        
        bitmapView->save("data/output/leader000_1.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/leader000_1.png") ==
                       RS_Img("data/expected/leader000_1.png"));

        RS_DEBUG->print(RS_Debug::D_TESTING, "testClick1: OK");
    }

    void testMove2() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMove2");
        
        doMouseMove(15,30);
        doMouseClick(15,30);
        doMouseMove(230,215);
        
        bitmapView->save("data/output/leader000_2.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/leader000_2.png") ==
                       RS_Img("data/expected/leader000_2.png"));
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMove2: OK");
    }

    void testClick2() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testClick2");

        doMouseMove(15,30);
        doMouseClick(15,30);
        doMouseMove(230,215);
        doMouseClick(230,215);
        doRightClick();
        
        bitmapView->save("data/output/leader000_3.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/leader000_3.png") ==
                       RS_Img("data/expected/leader000_3.png"));

        RS_Entity* e = ec.firstEntity();
        CPPUNIT_ASSERT( e!=NULL );
        CPPUNIT_ASSERT( e->rtti()==RS2::EntityDimLeader );
        //RS_Leader* l = (RS_Leader*)e;
        /*CPPUNIT_ASSERT( l->getStartpoint().distanceTo(RS_Vector(15,30)) 
            < RS_TOLERANCE );
        CPPUNIT_ASSERT( l->getEndpoint().distanceTo(RS_Vector(230,215)) 
            <  RS_TOLERANCE );*/
        
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
        doRightClick();
        doRightClick();
        
        bitmapView->save("data/output/leader001.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/leader001.png") ==
                       RS_Img("data/expected/leader001.png"));

        RS_DEBUG->print(RS_Debug::D_TESTING, "testSequence: OK");
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
        doRightClick();
        
        bitmapView->save("data/output/leader002.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/leader002.png") ==
                       RS_Img("data/expected/leader002.png"));

        RS_DEBUG->print(RS_Debug::D_TESTING, "testTwoSequences: OK");
    }
    
    void testTwoActions() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testTwoActions");

        doMouseClick(10,10);
        doMouseClick(200,30);
        doMouseClick(10,50);
        doRightClick();

        doRightClick();

        action = new RS_ActionDimLeader(ec, *bitmapView);
        bitmapView->setCurrentAction(action);
        
        doMouseClick(10,70);
        doMouseClick(200,90);
        doMouseClick(10,110);
        doRightClick();
        
        bitmapView->save("data/output/leader003.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/leader003.png") ==
                       RS_Img("data/expected/leader003.png"));

        RS_DEBUG->print(RS_Debug::D_TESTING, "testTwoActions: OK");
    }
    
    void testUndo() {
        RS_Entity* e;
        RS_BitmapGraphicView* bv;
        
        doMouseMove(10,10);
        doMouseClick(10,10);
        doMouseMove(200,200);
        doMouseClick(200,200);
        doMouseMove(240,200);
        doMouseClick(240,200);
        doRightClick();
        doRightClick();
        
        bv = new RS_BitmapGraphicView(250, 250);
        bv->setFactor(1.0);
        bv->setOffset(0,0);
        bv->save("data/output/leader004_0.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/leader004_0.png") ==
                       RS_Img("data/expected/leader004_0.png"));
        delete bv;

        e = ec.firstEntity();
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, e);
        CPPUNIT_ASSERT( e!=NULL );
        CPPUNIT_ASSERT( e->rtti()==RS2::EntityDimLeader );
        CPPUNIT_ASSERT( e->isUndone()==false );
        CPPUNIT_ASSERT( e->isVisible()==true );

        ec.undo();
        
        bv = new RS_BitmapGraphicView(250, 250);
        bv->setFactor(1.0);
        bv->setOffset(0,0);
        bv->save("data/output/leader004_1.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/leader004_1.png") ==
                       RS_Img("data/expected/leader004_1.png"));
        
        e = ec.firstEntity();
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, e);
        CPPUNIT_ASSERT( e!=NULL );
        CPPUNIT_ASSERT( e->rtti()==RS2::EntityDimLeader );
        CPPUNIT_ASSERT( e->isUndone()==true );
        CPPUNIT_ASSERT( e->isVisible()==false );
        
        ec.redo();
        
        bv = new RS_BitmapGraphicView(250, 250);
        bv->setFactor(1.0);
        bv->setOffset(0,0);
        bv->save("data/output/leader004_2.png", &ec, false);
        CPPUNIT_ASSERT(RS_Img("data/output/leader004_2.png") ==
                       RS_Img("data/expected/leader004_2.png"));
        
        e = ec.firstEntity();
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, e);
        CPPUNIT_ASSERT( e!=NULL );
        CPPUNIT_ASSERT( e->rtti()==RS2::EntityDimLeader );
        CPPUNIT_ASSERT( e->isUndone()==false );
        CPPUNIT_ASSERT( e->isVisible()==true );
    }

protected:
    RS_Graphic ec;
};
