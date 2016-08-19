#include <stdio.h>
#include <cppunit/extensions/HelperMacros.h>
#include "qg_colorbox.h"

class QG_ColorBoxTest : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( QG_ColorBoxTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_ColorBoxTest");
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_ColorBoxTest: OK");
    }
    
    void test() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "test");

        // init:
        QG_ColorBox* cb = new QG_ColorBox(NULL);
        
        // act:
        cb->setColor(RS_Color(255,0,0));
        CPPUNIT_ASSERT(cb->getColor()==RS_Color(255,0,0));

        RS_DEBUG->print(RS_Debug::D_TESTING, "test: OK");
    }
};
