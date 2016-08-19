#ifndef RS_MATHTEST
#define RS_MATHTEST

#include <stdio.h>
#include <cppunit/extensions/HelperMacros.h>
#include "rs_math.h"

#define TESTTOL 1.0e-10

class RS_MathTest : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( RS_MathTest );
    CPPUNIT_TEST( testEval );
    CPPUNIT_TEST( testRound );
    CPPUNIT_TEST( testDoubleToString );
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_MathTest");
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_MathTest: OK");
    }
    
    void testRound() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRound");
        CPPUNIT_ASSERT( RS_Math::mround(2.49)==2 );
        CPPUNIT_ASSERT( RS_Math::mround(2.5)==3 );
        CPPUNIT_ASSERT( RS_Math::mround(2.50)==3 );
        CPPUNIT_ASSERT( RS_Math::mround(2.51)==3 );
        CPPUNIT_ASSERT( RS_Math::mround(0.1)==0 );
        CPPUNIT_ASSERT( RS_Math::mround(0.5)==1 );
        CPPUNIT_ASSERT( RS_Math::mround(0.99)==1 );
        CPPUNIT_ASSERT( RS_Math::mround(-0.0)==0 );
        CPPUNIT_ASSERT( RS_Math::mround(-0)==0 );
        CPPUNIT_ASSERT( RS_Math::mround(0)==0 );
        CPPUNIT_ASSERT( RS_Math::mround(-0.001)==0 );
        CPPUNIT_ASSERT( RS_Math::mround(-0.1)==0 );
        CPPUNIT_ASSERT( RS_Math::mround(-0.5)==-1 );
        CPPUNIT_ASSERT( RS_Math::mround(-0.9)==-1 );
        CPPUNIT_ASSERT( RS_Math::mround(-2.5)==-3 );
        CPPUNIT_ASSERT( RS_Math::mround(20.5)==21 );
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRound: OK");
    }
    
    void testDoubleToString() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDoubleToString");

        CPPUNIT_ASSERT( RS_Math::doubleToString(0.0, 0)=="0" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(0.1, 0)=="0" );
        CPPUNIT_ASSERT_MESSAGE(
            (const char*)RS_Math::doubleToString(-0.1, 0).toLatin1(), 
            RS_Math::doubleToString(-0.1, 0)=="-0" );
        CPPUNIT_ASSERT_MESSAGE( 
            (const char*)RS_Math::doubleToString(0.5, 0).toLatin1(), 
            RS_Math::doubleToString(0.5, 0)=="1" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(0.9, 0)=="1" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(0.49, 0)=="0" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(0.51, 0)=="1" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(2.49, 0)=="2" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(2.51, 0)=="3" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(2.5, 0)=="3" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(2.5, 1)=="2.5" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(2.05, 1)=="2.1" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(2.04999999, 1)=="2" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(-2.49, 1)=="-2.5" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(-2.5, 1)=="-2.5" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(-2.51, 1)=="-2.5" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(-2.49, 0)=="-2" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(-2.5, 0)=="-3" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(-2.51, 0)=="-3" );

        CPPUNIT_ASSERT( RS_Math::doubleToString(0.1, 0.1)=="0.1" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(0.1, 0.01)=="0.1" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(0.1, 0.0)=="0" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(0.01, 0.1)=="0" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(0.01, 0.01)=="0.01" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(0.01, 0.0)=="0" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(0.001, 0.1)=="0" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(0.001, 0.01)=="0" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(0.001, 0.001)=="0.001" );
        CPPUNIT_ASSERT( RS_Math::doubleToString(0.001, 0.0)=="0" );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDoubleToString: OK");
    }

    void testEval() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testEval");
        CPPUNIT_ASSERT( RS_Math::eval("1+1")==2.0 );
        CPPUNIT_ASSERT( RS_Math::eval("2/2")==1.0 );
        CPPUNIT_ASSERT( RS_Math::eval("0")==0.0 );
        RS_DEBUG->print(RS_Debug::D_TESTING, "testEval: OK");
    }

};

#endif
