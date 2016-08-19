#include <stdio.h>
#include "rs_test.h"
#include "rs_line.h"

class RS_LeaderTest : public RS_Test  {
    CPPUNIT_TEST_SUITE( RS_LeaderTest );
    CPPUNIT_TEST( testRendering );
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_LeaderTest");
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_LineTest: OK");
    }
    
    void testRendering() {
        RS_EntityContainer ec;
        RS_Leader* leader = new RS_Leader(&ec, RS_LeaderData(true));
        leader->addVertex(RS_Vector(0,0));
        leader->addVertex(RS_Vector(30,30));
        ec.addEntity(leader);
        RS_BitmapGraphicView gv(100,100);
        saveAndCompare(&ec, "leader001", true);
    }

};
