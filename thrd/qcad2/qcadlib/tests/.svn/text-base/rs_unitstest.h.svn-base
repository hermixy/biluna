#ifndef RS_UNITSTEST
#define RS_UNITSTEST

#include <stdio.h>
#include <cppunit/extensions/HelperMacros.h>
#include "rs_math.h"

#define TESTTOL 1.0e-10

class RS_UnitsTest : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( RS_UnitsTest );
    CPPUNIT_TEST( testFormatLinearDecimal );
    CPPUNIT_TEST( testFormatLinearEngineering );
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_UnitsTest");
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_UnitsTest: OK");
    }

    void testFormatLinearDecimal() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testFormatLinearDecimal");

        RS_String s;
        s = RS_Units::formatLinear(0.0, 
                RS2::Millimeter, 
                RS2::Decimal, 
                2, false,
                false, false);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="0" );
        
        s = RS_Units::formatLinear(1.0, 
                RS2::Millimeter, 
                RS2::Decimal, 
                2, false,
                false, false);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="1" );
        
        s = RS_Units::formatLinear(1.1, 
                RS2::Millimeter, 
                RS2::Decimal, 
                2, false,
                false, false);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="1.1" );
        
        s = RS_Units::formatLinear(-1.0, 
                RS2::Millimeter, 
                RS2::Decimal, 
                2, false,
                false, false);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="-1" );
        
        s = RS_Units::formatLinear(-1.001, 
                RS2::Millimeter, 
                RS2::Decimal, 
                2, false,
                false, false);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="-1" );
        
        s = RS_Units::formatLinear(-1.009, 
                RS2::Millimeter, 
                RS2::Decimal, 
                2, false,
                false, false);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="-1.01" );
        
        s = RS_Units::formatLinear(-1.006, 
                RS2::Millimeter, 
                RS2::Decimal, 
                2, false,
                false, false);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="-1.01" );
        
        s = RS_Units::formatLinear(-1.016, 
                RS2::Millimeter, 
                RS2::Decimal, 
                2, false,
                false, false);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="-1.02" );
        
        s = RS_Units::formatLinear(-1.0151, 
                RS2::Millimeter, 
                RS2::Decimal, 
                2, false,
                false, false);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="-1.02" );
        
        s = RS_Units::formatLinear(-1.015, 
                RS2::Millimeter, 
                RS2::Decimal, 
                2, false,
                false, false);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="-1.02" );
        
        // show trailing zeroes:
        s = RS_Units::formatLinear(1.1, 
                RS2::Millimeter, 
                RS2::Decimal, 
                2, false,
                false, true);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="1.10" );
        
        s = RS_Units::formatLinear(1.11, 
                RS2::Millimeter, 
                RS2::Decimal, 
                2, false,
                false, true);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="1.11" );
        
        s = RS_Units::formatLinear(1.111, 
                RS2::Millimeter, 
                RS2::Decimal, 
                2, false,
                false, true);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="1.11" );
        
        s = RS_Units::formatLinear(0, 
                RS2::Millimeter, 
                RS2::Decimal, 
                2, false,
                false, true);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="0.00" );
        
        s = RS_Units::formatLinear(1.119, 
                RS2::Millimeter, 
                RS2::Decimal, 
                2, false,
                false, true);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="1.12" );
        
        s = RS_Units::formatLinear(1, 
                RS2::Millimeter, 
                RS2::Decimal, 
                2, false,
                false, true);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="1.00" );

        s = RS_Units::formatLinear(1, 
                RS2::Millimeter, 
                RS2::Decimal, 
                4, false,
                false, true);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="1.0000" );
        
        s = RS_Units::formatLinear(1, 
                RS2::Millimeter, 
                RS2::Decimal, 
                12, false,
                false, true);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT_MESSAGE((const char*)s.toLatin1(), s=="1.000000000000" );
        
        s = RS_Units::formatLinear(1.001, 
                RS2::Millimeter, 
                RS2::Decimal, 
                2, false,
                false, true);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="1.00" );
        
        s = RS_Units::formatLinear(1.001, 
                RS2::Millimeter, 
                RS2::Decimal, 
                3, false,
                false, true);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="1.001" );
        
        s = RS_Units::formatLinear(1.001, 
                RS2::Millimeter, 
                RS2::Decimal, 
                4, false,
                false, true);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="1.0010" );
        
        s = RS_Units::formatLinear(-1.001, 
                RS2::Millimeter, 
                RS2::Decimal, 
                4, false,
                false, true);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="-1.0010" );
        
        // with unit:
        s = RS_Units::formatLinear(1.001, 
                RS2::Millimeter, 
                RS2::Decimal, 
                4, true,
                false, true);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="1.0010mm" );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testFormatLinearDecimal: OK");
    }
        
    void testFormatLinearEngineering() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testFormatLinearEngineering");

        RS_String s;

        for (double d = 2.6; d<3.5; d+=0.1) {
            s = RS_Units::formatLinear(d,
                    RS2::Inch, 
                    RS2::Engineering, 
                    0, false);
            RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
            CPPUNIT_ASSERT( s=="3\"" );
        }
        
        s = RS_Units::formatLinear(1.0/4.0, 
                RS2::Foot, 
                RS2::Engineering, 
                2, false);
        RS_DEBUG->print(RS_Debug::D_TESTING, (const char*)s.toLatin1());
        CPPUNIT_ASSERT( s=="" );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testFormatLinearEngineering: OK");
    }

};

#endif
