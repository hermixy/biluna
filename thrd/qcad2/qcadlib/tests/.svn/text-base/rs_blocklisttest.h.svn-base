#include <stdio.h>
#include <cppunit/extensions/HelperMacros.h>
#include "rs_block.h"
#include "rs_blocklist.h"
#include "rs_blocklistlistener.h"

RS_String addedBlock;

class RS_DummyListener : public RS_BlockListListener {
public:
    virtual void blockAdded(RS_Block* b) {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_DummyListener::blockAdded");
        CPPUNIT_ASSERT( b!=NULL );
        addedBlock = b->getName();
    }
};


class RS_BlockListTest : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( RS_BlockListTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_BlockListTest");
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_BlockListTest: OK");
    }
    
    void test() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "test");

        RS_BlockList bl(true);
        RS_DummyListener l;
        bl.addListener(&l);
        
        addedBlock = "";
        bl.add(new RS_Block(NULL, RS_BlockData("block1", RS_Vector(0,0), false)));
        CPPUNIT_ASSERT( bl.count()==1 );
        CPPUNIT_ASSERT( addedBlock=="block1" );
        
        bl.add(new RS_Block(NULL, RS_BlockData("block2", RS_Vector(0,0), false)));
        CPPUNIT_ASSERT( bl.count()==2 );
        CPPUNIT_ASSERT( addedBlock=="block2" );

        RS_Block* block = bl.find("block1");
        CPPUNIT_ASSERT( block!=NULL );
        CPPUNIT_ASSERT( block->getName()=="block1" );

        bl.activate("block2");
        CPPUNIT_ASSERT( bl.getActive()!=NULL );
        CPPUNIT_ASSERT( bl.getActive()->getName()=="block2" );

        CPPUNIT_ASSERT( bl.at(2)==NULL );
        CPPUNIT_ASSERT( bl.at(1)!=NULL );
        CPPUNIT_ASSERT( bl.at(1)->getName()=="block2" );
        CPPUNIT_ASSERT( bl.at(0)!=NULL );
        CPPUNIT_ASSERT( bl.at(0)->getName()=="block1" );
        CPPUNIT_ASSERT( bl.at(-1)==NULL );

        RS_DEBUG->print("remove 1");
        bl.remove(bl.at(0));
        RS_DEBUG->print("remove 2");
        bl.remove(bl.at(0));
        RS_DEBUG->print("count");
        CPPUNIT_ASSERT( bl.count()==0 );

        RS_DEBUG->print("clear");
        bl.clear();
        CPPUNIT_ASSERT( bl.count()==0 );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "test: OK");
    }
};


