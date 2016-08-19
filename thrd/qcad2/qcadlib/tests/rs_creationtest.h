#include "rs_test.h"
#include "rs_creation.h"
#include "rs_fileinfo.h"

class RS_CreationTest : public RS_Test {
    CPPUNIT_TEST_SUITE( RS_CreationTest );
    CPPUNIT_TEST_SUITE_END();

private:
    RS_EntityContainer* ec;
    RS_Creation* creation;
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_CreationTest");
        ec = new RS_EntityContainer();
        creation = new RS_Creation(ec);
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_CreationTest: OK");
        delete ec;
        delete creation;
        ec = NULL;
        creation = NULL;
    }
    
};
