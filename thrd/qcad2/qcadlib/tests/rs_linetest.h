#include <stdio.h>
#include "rs_test.h"
#include "rs_line.h"

class RS_LineTest : public RS_Test  {
    CPPUNIT_TEST_SUITE( RS_LineTest );
    CPPUNIT_TEST( testGetSideOfPoint );
    CPPUNIT_TEST( testGetDistanceToPoint );
    CPPUNIT_TEST( testRendering );
    CPPUNIT_TEST_SUITE_END();

private:
    RS_Line** l;
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_LineTest");
        l = new RS_Line*[16];
        double ang;
        double x, y;
        for (int i=0; i<16; ++i) {
            ang = 2*M_PI/16*i;
            x = 10*cos(ang);
            y = 10*sin(ang);
            l[i] = new RS_Line(NULL, RS_LineData(RS_Vector(0,0), RS_Vector(x,y)));
        }
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_LineTest: OK");
        for (int i=0; i<16; ++i) {
            delete l[i];
        }
        delete[] l;
        l = NULL;
    }
    
    void testGetSideOfPoint() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testGetSideOfPoint");
        CPPUNIT_ASSERT( l[ 0]->getSideOfPoint(RS_Vector(-5,2))==RS2::LeftHand );
        CPPUNIT_ASSERT( l[ 0]->getSideOfPoint(RS_Vector(5,2))==RS2::LeftHand );
        CPPUNIT_ASSERT( l[ 0]->getSideOfPoint(RS_Vector(10,2))==RS2::LeftHand );
        CPPUNIT_ASSERT( l[ 0]->getSideOfPoint(RS_Vector(10e5,2))==RS2::LeftHand );
        
        CPPUNIT_ASSERT( l[ 0]->getSideOfPoint(RS_Vector(-5,-2))==RS2::RightHand );
        CPPUNIT_ASSERT( l[ 0]->getSideOfPoint(RS_Vector(5,-2))==RS2::RightHand );
        CPPUNIT_ASSERT( l[ 0]->getSideOfPoint(RS_Vector(10,-2))==RS2::RightHand );
        CPPUNIT_ASSERT( l[ 0]->getSideOfPoint(RS_Vector(10e5,-2))==RS2::RightHand );
        
        CPPUNIT_ASSERT( l[ 2]->getSideOfPoint(RS_Vector(0,10))==RS2::LeftHand );
        CPPUNIT_ASSERT( l[ 2]->getSideOfPoint(RS_Vector(10,0))==RS2::RightHand );
        
        CPPUNIT_ASSERT( l[ 4]->getSideOfPoint(RS_Vector(-2,5))==RS2::LeftHand );
        CPPUNIT_ASSERT( l[ 4]->getSideOfPoint(RS_Vector(2,5))==RS2::RightHand );
        
        CPPUNIT_ASSERT( l[ 6]->getSideOfPoint(RS_Vector(-10,0))==RS2::LeftHand );
        CPPUNIT_ASSERT( l[ 6]->getSideOfPoint(RS_Vector(0,10))==RS2::RightHand );
        
        CPPUNIT_ASSERT( l[ 8]->getSideOfPoint(RS_Vector(-5,-2))==RS2::LeftHand );
        CPPUNIT_ASSERT( l[ 8]->getSideOfPoint(RS_Vector(-5,2))==RS2::RightHand );
        
        CPPUNIT_ASSERT( l[10]->getSideOfPoint(RS_Vector(0,-10))==RS2::LeftHand );
        CPPUNIT_ASSERT( l[10]->getSideOfPoint(RS_Vector(-10,0))==RS2::RightHand );
        
        CPPUNIT_ASSERT( l[12]->getSideOfPoint(RS_Vector(2,-5))==RS2::LeftHand );
        CPPUNIT_ASSERT( l[12]->getSideOfPoint(RS_Vector(-2,-5))==RS2::RightHand );
        
        CPPUNIT_ASSERT( l[14]->getSideOfPoint(RS_Vector(10,0))==RS2::LeftHand );
        CPPUNIT_ASSERT( l[14]->getSideOfPoint(RS_Vector(0,-10))==RS2::RightHand );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testGetSideOfPoint: OK");
    }

    void testGetDistanceToPoint() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testGetDistanceToPoint");
        
        RS_Vector coord;
        double dist;
        RS_Line line(NULL, 
            RS_LineData(RS_Vector(2.0,1.0),  
                RS_Vector(1.0,1.0)));

        coord = RS_Vector(1.5, 1.1);
        dist = line.getDistanceToPoint(coord);

        CPPUNIT_ASSERT( dist < 0.11 );
        
        coord = RS_Vector(1.5, 0.9);
        dist = line.getDistanceToPoint(coord);
        
        CPPUNIT_ASSERT( dist < 0.11 );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testGetDistanceToPoint: OK");
    }

    void testRendering() {
        RS_EntityContainer ec;
        ec.addEntity(new RS_Line(&ec, RS_LineData(RS_Vector(0,0), RS_Vector(100,100))));
        saveAndCompare(&ec, "line001");
    }
};
