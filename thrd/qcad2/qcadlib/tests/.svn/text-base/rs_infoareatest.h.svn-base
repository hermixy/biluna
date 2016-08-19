#include <stdio.h>
#include <cppunit/extensions/HelperMacros.h>
#include "rs_infoarea.h"

class RS_InfoAreaTest : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( RS_InfoAreaTest );
    CPPUNIT_TEST( testValid );
    CPPUNIT_TEST( testInvalid01 );
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_InfoAreaTest");
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_InfoAreaTest: OK");
    }
    
    void testValid() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testValid");
        
        RS_InfoArea ia;
        ia.addPoint(RS_Vector(10,10));
        ia.addPoint(RS_Vector(20,10));
        ia.addPoint(RS_Vector(20,20));
        ia.addPoint(RS_Vector(10,20));
        ia.calculate();
        CPPUNIT_ASSERT( fabs(ia.getArea()-100)<1.0e-10 );
        CPPUNIT_ASSERT( fabs(ia.getCircumference()-40)<1.0e-10 );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testValid: OK");
    }
    
    void testInvalid01() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testInvalid01");
        
        RS_InfoArea ia;
        ia.addPoint(RS_Vector(10,10));
        ia.calculate();
        CPPUNIT_ASSERT( fabs(ia.getArea())<1.0e-10 );
        CPPUNIT_ASSERT( fabs(ia.getCircumference())<1.0e-10 );
        
        ia.addPoint(RS_Vector(20,10));
        ia.calculate();
        CPPUNIT_ASSERT( fabs(ia.getArea())<1.0e-10 );
        CPPUNIT_ASSERT( fabs(ia.getCircumference())<1.0e-10 );
        
        ia.addPoint(RS_Vector(20,20));
        ia.calculate();
        CPPUNIT_ASSERT( fabs(ia.getArea()-50)<1.0e-10 );
        CPPUNIT_ASSERT( fabs(ia.getCircumference()-34.14213562373095)<1.0e-10 );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testInvalid01: OK");
    }
};

