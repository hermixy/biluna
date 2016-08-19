#ifndef RS_MATRIXTEST
#define RS_MATRIXTEST

#include <stdio.h>
#include <cppunit/extensions/HelperMacros.h>
#include "rs_matrix.h"
#include "rs_debug.h"

#define TESTTOL 1.0e-10

class RS_MatrixTest : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( RS_MatrixTest );
    CPPUNIT_TEST( testOperators );
    CPPUNIT_TEST( testMultiplication );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_MatrixTest");
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_MatrixTest: OK");
    }
    
    void testOperators() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testOperators");
    
        RS_Matrix a(3,2);

        a.set(0,0,3);
        a.set(0,1,2);
        a.set(1,0,1);
        a.set(1,1,5);
        a.set(2,0,1);
        a.set(2,1,5);

        RS_Matrix b = a;
        RS_Matrix c(a);
        RS_Matrix d(1,1);
        d = a;

        CPPUNIT_ASSERT(a==b);
        CPPUNIT_ASSERT(b==a);
        CPPUNIT_ASSERT(a==c);
        CPPUNIT_ASSERT(a==d);

        RS_Matrix e = a*2;
        CPPUNIT_ASSERT(e.get(0,0)==6);
        CPPUNIT_ASSERT(e.get(0,1)==4);
        CPPUNIT_ASSERT(e.get(1,0)==2);
        CPPUNIT_ASSERT(e.get(1,1)==10);
        CPPUNIT_ASSERT(e.get(2,0)==2);
        CPPUNIT_ASSERT(e.get(2,1)==10);
        
        RS_Matrix f = 2*a;
        CPPUNIT_ASSERT(f.get(0,0)==6);
        CPPUNIT_ASSERT(f.get(0,1)==4);
        CPPUNIT_ASSERT(f.get(1,0)==2);
        CPPUNIT_ASSERT(f.get(1,1)==10);
        CPPUNIT_ASSERT(f.get(2,0)==2);
        CPPUNIT_ASSERT(f.get(2,1)==10);

        a*=2;
        CPPUNIT_ASSERT(a.get(0,0)==6);
        CPPUNIT_ASSERT(a.get(0,1)==4);
        CPPUNIT_ASSERT(a.get(1,0)==2);
        CPPUNIT_ASSERT(a.get(1,1)==10);
        CPPUNIT_ASSERT(a.get(2,0)==2);
        CPPUNIT_ASSERT(a.get(2,1)==10);
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testOperators: OK");
    }
    
    void testMultiplication() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMultiplication");
    
        RS_Matrix a(3,2);
        RS_Matrix b(2,3);

        a.set(0,0,3);
        a.set(0,1,2);
        a.set(1,0,1);
        a.set(1,1,5);
        a.set(2,0,1);
        a.set(2,1,5);

        b.set(0,0,6);
        b.set(0,1,0);
        b.set(0,2,4);
        b.set(1,0,7);
        b.set(1,1,3);
        b.set(1,2,4);

        RS_Matrix c = a.multiplyWith(b);

        CPPUNIT_ASSERT(fabs(c.get(0,0)-32)<TESTTOL);
        CPPUNIT_ASSERT(fabs(c.get(0,1)-6)<TESTTOL);
        CPPUNIT_ASSERT(fabs(c.get(0,2)-20)<TESTTOL);
        CPPUNIT_ASSERT(fabs(c.get(1,0)-41)<TESTTOL);
        CPPUNIT_ASSERT(fabs(c.get(1,1)-15)<TESTTOL);
        CPPUNIT_ASSERT(fabs(c.get(1,2)-24)<TESTTOL);
        CPPUNIT_ASSERT(fabs(c.get(2,0)-41)<TESTTOL);
        CPPUNIT_ASSERT(fabs(c.get(2,1)-15)<TESTTOL);
        CPPUNIT_ASSERT(fabs(c.get(2,2)-24)<TESTTOL);
        
        a.set3x3(6, 1, 5,
                 7, 5, 9,
                 5, 1, 5);
        
        b.set3x3(2, 5, 8,
                 0, 4, 4,
                 9, 1, 4);
        
        c = a.multiplyWith(b);
        
        CPPUNIT_ASSERT(fabs(c.get(0,0)-57)<TESTTOL);
       
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMultiplication: OK");
    }
    
};

#endif
