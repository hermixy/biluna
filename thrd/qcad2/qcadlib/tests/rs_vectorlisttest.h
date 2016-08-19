#ifndef RS_VECTORLISTTEST
#define RS_VECTORLISTTEST

#include <stdio.h>
#include <cppunit/extensions/HelperMacros.h>
#include "rs_vector.h"

#define TESTTOL 1.0e-10

class RS_VectorListTest : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( RS_VectorListTest );
    CPPUNIT_TEST( testAllocation );
    CPPUNIT_TEST( testEquality );
    CPPUNIT_TEST( testAddition );
    CPPUNIT_TEST( testSortByDistanceTo );
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_VectorListTest");
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_VectorListTest: OK");
    }
    
    void testAllocation() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_VectorListTest::testAllocation");
        
        RS_VectorList vs;
        vs.set(10, RS_Vector(10,10));
        CPPUNIT_ASSERT(vs[10]==RS_Vector(10,10));
        CPPUNIT_ASSERT(!vs[9].valid);
        CPPUNIT_ASSERT(vs.count()==11);
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_VectorListTest::testAllocation: OK");
    }

    void testEquality() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_VectorListTest::testEquality");

        RS_VectorList vs1(RS_Vector(4,5), RS_Vector(6,7));
        RS_VectorList vs2(RS_Vector(4,5), RS_Vector(6,7));
        RS_VectorList vs3(RS_Vector(4,5), RS_Vector(6,0));
        RS_VectorList vs4(RS_Vector(4,5), RS_Vector(6,0), RS_Vector(1,2));
        CPPUNIT_ASSERT(vs1==vs2);
        CPPUNIT_ASSERT(vs1!=vs3);
        CPPUNIT_ASSERT(vs3==vs3);
        CPPUNIT_ASSERT(vs3!=vs4);
        CPPUNIT_ASSERT(vs4==vs4);

        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_VectorListTest::testEquality: OK");
    }
    
    void testAddition() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_VectorListTest::testAddition");

        RS_VectorList vs1(RS_Vector(1,2), RS_Vector(3,4));
        RS_VectorList vs2(RS_Vector(5,6), RS_Vector(7,8));
        RS_VectorList vs3 = vs1+vs2;

        CPPUNIT_ASSERT(vs3.count()==4);
        CPPUNIT_ASSERT(vs3.get(0)==RS_Vector(1,2));
        CPPUNIT_ASSERT(vs3.get(1)==RS_Vector(3,4));
        CPPUNIT_ASSERT(vs3.get(2)==RS_Vector(5,6));
        CPPUNIT_ASSERT(vs3.get(3)==RS_Vector(7,8));

        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_VectorListTest::testAddition: OK");
    }
    
    void testSortByDistanceTo() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_VectorListTest::testSortByDistanceTo");

        RS_VectorList vs(RS_Vector(2,2), RS_Vector(5,5), RS_Vector(3,3), RS_Vector(3,3));
        
        CPPUNIT_ASSERT(vs.get(0)==RS_Vector(2,2));
        CPPUNIT_ASSERT(vs.get(1)==RS_Vector(5,5));
        CPPUNIT_ASSERT(vs.get(2)==RS_Vector(3,3));
        CPPUNIT_ASSERT(vs.get(3)==RS_Vector(3,3));
        
        vs.sortByDistanceTo(RS_Vector(0,0));

        CPPUNIT_ASSERT(vs.get(0)==RS_Vector(2,2));
        CPPUNIT_ASSERT(vs.get(1)==RS_Vector(3,3));
        CPPUNIT_ASSERT(vs.get(2)==RS_Vector(3,3));
        CPPUNIT_ASSERT(vs.get(3)==RS_Vector(5,5));
        
        vs.sortByDistanceTo(RS_Vector(6,6));
        
        CPPUNIT_ASSERT(vs.get(0)==RS_Vector(5,5));
        CPPUNIT_ASSERT(vs.get(1)==RS_Vector(3,3));
        CPPUNIT_ASSERT(vs.get(2)==RS_Vector(3,3));
        CPPUNIT_ASSERT(vs.get(3)==RS_Vector(2,2));
        
        vs.sortByDistanceTo(RS_Vector(0,0), true);
        
        CPPUNIT_ASSERT(vs.get(0)==RS_Vector(2,2));
        CPPUNIT_ASSERT(vs.get(1)==RS_Vector(3,3));
        CPPUNIT_ASSERT(vs.get(2)==RS_Vector(5,5));

        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_VectorListTest::testSortByDistanceTo: OK");
    }
};

#endif
