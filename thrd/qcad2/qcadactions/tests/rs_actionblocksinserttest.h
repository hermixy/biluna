#include <stdio.h>
#include "rs_test.h"
#include "rs_line.h"
#include "rs_actionblocksinsert.h"
#include "rs_bitmapgraphicview.h"

class RS_ActionBlocksInsertTest : public RS_Test {
    CPPUNIT_TEST_SUITE( RS_ActionBlocksInsertTest );
    CPPUNIT_TEST( testMoveSingle );
    CPPUNIT_TEST( testClickSingle );
    
    CPPUNIT_TEST( testMoveNested );
    CPPUNIT_TEST( testClickNested );
    CPPUNIT_TEST_SUITE_END();

private:
    RS_ActionBlocksInsert* action;
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_ActionBlocksInsertTest");
        RS_Test::setUp();
        
        // create and add a block with one line
        RS_Block* singleBlock = new RS_Block(container, 
            RS_BlockData("singleBlock", RS_Vector(0,0), false));
        RS_Line* line = new RS_Line(singleBlock, 
            RS_LineData(RS_Vector(0,0), RS_Vector(10,10)));
        singleBlock->addEntity(line);
        container->addBlock(singleBlock);
        
        // create and add another block with one line and an 
        // insert of the first block
        RS_Block* nestedBlock = new RS_Block(container, 
            RS_BlockData("nestedBlock", RS_Vector(0,0), false));
        line = new RS_Line(nestedBlock, 
            RS_LineData(RS_Vector(0,0), RS_Vector(-100,100)));
        nestedBlock->addEntity(line);
        nestedBlock->addEntity(new RS_Insert(container, 
            RS_InsertData("singleBlock", RS_Vector(0,0),
                RS_Vector(1.0,1.0), 0.0, 1,1, 
                RS_Vector(0.0,0.0))));
        container->addBlock(nestedBlock);
        
    }

    void tearDown() {
        RS_Test::tearDown();
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_ActionBlocksInsertTest: OK");
    }

    void testMoveSingle() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMoveSingle");
        
        container->activateBlock("singleBlock");
        action = new RS_ActionBlocksInsert(*container, *bitmapView);
        bitmapView->setCurrentAction(action);
        
        doMouseMove(125,125);

        bitmapView->save("data/output/blocksinsert000_0.png");
        CPPUNIT_ASSERT(RS_Img("data/output/blocksinsert000_0.png") ==
                       RS_Img("data/expected/blocksinsert000_0.png"));
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMoveSingle: OK");
    }

    void testClickSingle() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testClickSingle");
        
        container->activateBlock("singleBlock");
        action = new RS_ActionBlocksInsert(*container, *bitmapView);
        bitmapView->setCurrentAction(action);

        doMouseMove(125,125);
        doMouseClick(125,125);
        
        bitmapView->save("data/output/blocksinsert000_1.png");
        CPPUNIT_ASSERT(RS_Img("data/output/blocksinsert000_1.png") ==
                       RS_Img("data/expected/blocksinsert000_1.png"));

        RS_DEBUG->print(RS_Debug::D_TESTING, "testClickSingle: OK");
    }
    
    void testMoveNested() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMoveNested");
        
        container->activateBlock("nestedBlock");
        action = new RS_ActionBlocksInsert(*container, *bitmapView);
        bitmapView->setCurrentAction(action);
        
        doMouseMove(125,125);
        doMouseMove(155,155);
        doMouseMove(100,30);

        bitmapView->save("data/output/blocksinsert001_0.png");
        CPPUNIT_ASSERT(RS_Img("data/output/blocksinsert001_0.png") ==
                       RS_Img("data/expected/blocksinsert001_0.png"));
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMoveNested: OK");
    }

    void testClickNested() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testClickNested");
        
        container->activateBlock("nestedBlock");
        action = new RS_ActionBlocksInsert(*container, *bitmapView);
        bitmapView->setCurrentAction(action);

        doMouseMove(125,125);
        doMouseClick(125,125);
        
        bitmapView->save("data/output/blocksinsert001_1.png");
        CPPUNIT_ASSERT(RS_Img("data/output/blocksinsert001_1.png") ==
                       RS_Img("data/expected/blocksinsert001_1.png"));

        RS_DEBUG->print(RS_Debug::D_TESTING, "testClickNested: OK");
    }
    
protected:
    RS_EntityContainer ec;
};
