#include <stdio.h>
#include <cppunit/extensions/HelperMacros.h>
#include "rs_arc.h"

class RS_ArcTest : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( RS_ArcTest );
    CPPUNIT_TEST( testGetSideOfPoint );
    CPPUNIT_TEST_SUITE_END();

private:
    RS_Arc** a1;
    RS_Arc** a2;
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_ArcTest");
        double ang;
        a1 = new RS_Arc*[16];
        for (int i=0; i<16; ++i) {
            ang = 2*M_PI/16*i;
            a1[i] = new RS_Arc(NULL, RS_ArcData(RS_Vector(0,0), 10, ang, ang+M_PI/6, false));
        }
        a2 = new RS_Arc*[16];
        for (int i=0; i<16; ++i) {
            a2[i] = new RS_Arc(NULL, RS_ArcData(RS_Vector(0,0), 10, ang, ang-M_PI/6, true));
        }
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_ArcTest: OK");
        for (int i=0; i<16; ++i) {
            delete a1[i];
            delete a2[i];
        }
        delete[] a1;
        delete[] a2;
        a1 = NULL;
        a2 = NULL;
    }
    
    void testGetSideOfPoint() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testGetSideOfPoint");
        
        for (int i=0; i<16; ++i) {
            CPPUNIT_ASSERT( a1[i]->getSideOfPoint(RS_Vector(9,0))==RS2::LeftHand );
            CPPUNIT_ASSERT( a1[i]->getSideOfPoint(RS_Vector(11,0))==RS2::RightHand );
            
            CPPUNIT_ASSERT( a1[i]->getSideOfPoint(RS_Vector(5,5))==RS2::LeftHand );
            CPPUNIT_ASSERT( a1[i]->getSideOfPoint(RS_Vector(10,10))==RS2::RightHand );
            
            CPPUNIT_ASSERT( a1[i]->getSideOfPoint(RS_Vector(-5,-5))==RS2::LeftHand );
            CPPUNIT_ASSERT( a1[i]->getSideOfPoint(RS_Vector(-10,-10))==RS2::RightHand );
        }
        
        for (int i=0; i<16; ++i) {
            CPPUNIT_ASSERT( a2[i]->getSideOfPoint(RS_Vector(9,0))==RS2::RightHand );
            CPPUNIT_ASSERT( a2[i]->getSideOfPoint(RS_Vector(11,0))==RS2::LeftHand );
            
            CPPUNIT_ASSERT( a2[i]->getSideOfPoint(RS_Vector(5,5))==RS2::RightHand );
            CPPUNIT_ASSERT( a2[i]->getSideOfPoint(RS_Vector(10,10))==RS2::LeftHand );
            
            CPPUNIT_ASSERT( a2[i]->getSideOfPoint(RS_Vector(-5,-5))==RS2::RightHand );
            CPPUNIT_ASSERT( a2[i]->getSideOfPoint(RS_Vector(-10,-10))==RS2::LeftHand );
        }
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testGetSideOfPoint: OK");
    }
};
