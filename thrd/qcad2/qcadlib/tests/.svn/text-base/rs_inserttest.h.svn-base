#include <stdio.h>
#include "rs_test.h"
#include "rs_line.h"

class RS_InsertTest : public RS_Test  {
    CPPUNIT_TEST_SUITE( RS_InsertTest );
    CPPUNIT_TEST( testUpdate );
    CPPUNIT_TEST( testRendering );
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_InsertTest");
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_InsertTest: OK");
    }
    
    void testUpdate() {
        RS_EntityContainer* ec = new RS_EntityContainer(NULL);
        RS_BlockList* blkList = new RS_BlockList(true);
        RS_Block* blk = new RS_Block(ec, 
            RS_BlockData("blk01", RS_Vector(0,0), false));
        RS_Line* l = new RS_Line(blk, 
            RS_LineData(RS_Vector(0,0), RS_Vector(100,100)));
        blk->addEntity(l);
        blkList->add(blk);
        
        RS_Insert* ins = new RS_Insert(ec, 
            RS_InsertData("blk01", 
                RS_Vector(0,0),
                RS_Vector(1,1),
                0.0, 
                1, 1, 
                RS_Vector(0,0),
                blkList));
        ec->addEntity(ins);

        delete ec;
        delete blkList;
    }

    void testRendering() {
    }
};
