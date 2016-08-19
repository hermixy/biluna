#ifndef RS_VECTORTEST
#define RS_VECTORTEST

#include <stdio.h>
#include <cppunit/extensions/HelperMacros.h>
#include "rs_vector.h"

#define TESTTOL 1.0e-10

class RS_VectorTest : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( RS_VectorTest );
    CPPUNIT_TEST( testEquality );
    CPPUNIT_TEST( testPolar );
    CPPUNIT_TEST( testDistanceTo );
    CPPUNIT_TEST( testAngle );
    CPPUNIT_TEST( testAngleTo );
    CPPUNIT_TEST( testAngleToPlaneXY );
    CPPUNIT_TEST( testMagnitude );
    CPPUNIT_TEST( testIsInWindow );
    CPPUNIT_TEST( testRotate );
    CPPUNIT_TEST( testIsoProject );
    CPPUNIT_TEST_SUITE_END();

private:
    RS_Vector v[5];
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_VectorTest");
        v[0] = RS_Vector(0, 0);
        v[1] = RS_Vector(10, 5);
        v[2] = RS_Vector(12.5, -7.3);
        v[3] = RS_Vector(-4235, -346);
        v[4] = RS_Vector(10e10, 72e8);
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_VectorTest: OK");
    }

    void testEquality() {
        for (int i=0; i<5; ++i) {
            CPPUNIT_ASSERT( v[i]==v[i] );
            for (int k=0; k<5; ++k) {
                if (i!=k) {
                    CPPUNIT_ASSERT( v[i]!=v[k] );
                }
            }
        }
    }

    void testPolar() {
        RS_Vector v;
        v.setPolar(10, 0.5);
        CPPUNIT_ASSERT(v.distanceTo(
            RS_Vector(8.775825618903727, 4.79425538604203)) < TESTTOL);
        v.setPolar(10, -0.5);
        CPPUNIT_ASSERT(v.distanceTo(
            RS_Vector(8.775825618903727, -4.79425538604203)) < TESTTOL);
        v.setPolar(10, M_PI-0.5);
        CPPUNIT_ASSERT(v.distanceTo(
            RS_Vector(-8.775825618903727, 4.79425538604203)) < TESTTOL);
        v.setPolar(10, M_PI+0.5);
        CPPUNIT_ASSERT(v.distanceTo(
            RS_Vector(-8.775825618903727, -4.79425538604203)) < TESTTOL);
    }
    
    void testDistanceTo() {
        RS_Vector v0(0, 0);
        
        RS_Vector v1(4,5);
        RS_Vector v2(-4,5);
        RS_Vector v3(-4,-5);
        RS_Vector v4(4,-5);
        
        CPPUNIT_ASSERT(v1.distanceTo(v1) < TESTTOL);
        
        CPPUNIT_ASSERT(v1.distanceTo(v2)-8 < TESTTOL);
        CPPUNIT_ASSERT(v2.distanceTo(v3)-10 < TESTTOL);
        CPPUNIT_ASSERT(v3.distanceTo(v4)-8 < TESTTOL);
        CPPUNIT_ASSERT(v4.distanceTo(v1)-10 < TESTTOL);
        
        CPPUNIT_ASSERT(v1.distanceTo(v3)-12.8062484748657 < TESTTOL);
        CPPUNIT_ASSERT(v2.distanceTo(v4)-12.8062484748657 < TESTTOL);
        
        CPPUNIT_ASSERT(v0.distanceTo(v1)-6.403124237432849 < TESTTOL);
        CPPUNIT_ASSERT(v0.distanceTo(v0) < TESTTOL);
        CPPUNIT_ASSERT(v0.distanceTo(RS_Vector(10e6,0))-10e6 < TESTTOL);
    }
    
    void testAngle() {
        RS_Vector v0(0, 0);
        
        RS_Vector v1(4,5);
        RS_Vector v2(-4,5);
        RS_Vector v3(-4,-5);
        RS_Vector v4(4,-5);
        
        CPPUNIT_ASSERT(v0.angle() < TESTTOL);
        
        CPPUNIT_ASSERT(v1.angle()-0.896055384571344 < TESTTOL);
        CPPUNIT_ASSERT(v2.angle()-(M_PI-0.896055384571344) < TESTTOL);
        CPPUNIT_ASSERT(v3.angle()-(M_PI+0.896055384571344) < TESTTOL);
        CPPUNIT_ASSERT(v4.angle()-(2*M_PI-0.896055384571344) < TESTTOL);
        
        v1 = RS_Vector(5,0);
        v2 = RS_Vector(0,5);
        v3 = RS_Vector(-5,0);
        v4 = RS_Vector(0,-5);
        CPPUNIT_ASSERT(v1.angle() < TESTTOL);
        CPPUNIT_ASSERT(v2.angle()-M_PI/2 < TESTTOL);
        CPPUNIT_ASSERT(v3.angle()-M_PI < TESTTOL);
        CPPUNIT_ASSERT(v4.angle()-M_PI/2*3 < TESTTOL);
    }
    
    void testAngleTo() {
        RS_Vector v0(0, 0);
        
        RS_Vector v1(4,5);
        RS_Vector v2(-4,5);
        RS_Vector v3(-4,-5);
        RS_Vector v4(4,-5);
        
        CPPUNIT_ASSERT(v0.angleTo(v0) < TESTTOL);
        
        CPPUNIT_ASSERT(fabs(v1.angleTo(v2)-M_PI) < TESTTOL);
        CPPUNIT_ASSERT(fabs(v2.angleTo(v3)-M_PI/2*3) < TESTTOL);
        CPPUNIT_ASSERT(v3.angleTo(v4) < TESTTOL);
        CPPUNIT_ASSERT(fabs(v4.angleTo(v1)-M_PI/2) < TESTTOL);
    }
    
    void testAngleToPlaneXY() {
        RS_Vector v;

        v = RS_Vector(3,0,3);
        CPPUNIT_ASSERT(fabs(v.angleToPlaneXY() - M_PI/4) < TESTTOL);
        
        v = RS_Vector(0,3,3);
        CPPUNIT_ASSERT(fabs(v.angleToPlaneXY() - M_PI/4) < TESTTOL);
        
        v = RS_Vector(-3,0,3);
        CPPUNIT_ASSERT(fabs(v.angleToPlaneXY() - M_PI/4) < TESTTOL);
        
        v = RS_Vector(0,-3,3);
        CPPUNIT_ASSERT(fabs(v.angleToPlaneXY() - M_PI/4) < TESTTOL);
        
        v = RS_Vector(0,3,-3);
        CPPUNIT_ASSERT(fabs(v.angleToPlaneXY() + M_PI/4) < TESTTOL);
        
        v = RS_Vector(0,-3,-3);
        CPPUNIT_ASSERT(fabs(v.angleToPlaneXY() + M_PI/4) < TESTTOL);
        
        v = RS_Vector(-3,0,-3);
        CPPUNIT_ASSERT(fabs(v.angleToPlaneXY() + M_PI/4) < TESTTOL);
        
        v = RS_Vector(1,1,1);
        CPPUNIT_ASSERT(fabs(v.angleToPlaneXY() - 0.61547970867) < TESTTOL);
        
        v = RS_Vector(0,0,1);
        CPPUNIT_ASSERT(fabs(v.angleToPlaneXY() - M_PI/2) < TESTTOL);
        
        v = RS_Vector(1,0,0);
        CPPUNIT_ASSERT(fabs(v.angleToPlaneXY()) < TESTTOL);
        
        v = RS_Vector(0,1,0);
        CPPUNIT_ASSERT(fabs(v.angleToPlaneXY()) < TESTTOL);
        
        v = RS_Vector(-1,0,0);
        CPPUNIT_ASSERT(fabs(v.angleToPlaneXY()) < TESTTOL);
        
        v = RS_Vector(0,-1,0);
        CPPUNIT_ASSERT(fabs(v.angleToPlaneXY()) < TESTTOL);
    }
    
    void testMagnitude() {
        RS_Vector v0(0, 0);
        
        RS_Vector v1(4,5);
        RS_Vector v2(-4,5);
        RS_Vector v3(-4,-5);
        RS_Vector v4(4,-5);
        
        CPPUNIT_ASSERT(v0.magnitude() < TESTTOL);
        
        CPPUNIT_ASSERT(v1.magnitude()-6.403124237432849 < TESTTOL);
        CPPUNIT_ASSERT(v2.magnitude()-6.403124237432849 < TESTTOL);
        CPPUNIT_ASSERT(v3.magnitude()-6.403124237432849 < TESTTOL);
        CPPUNIT_ASSERT(v4.magnitude()-6.403124237432849 < TESTTOL);
    }
    
    void testIsInWindow() {
        RS_Vector c1(2,3);
        RS_Vector c2(5,7);

        CPPUNIT_ASSERT(!RS_Vector(0,0).isInWindow(c1, c2));
        CPPUNIT_ASSERT(!RS_Vector(1.999,2.999).isInWindow(c1, c2));
        CPPUNIT_ASSERT(!RS_Vector(3,8).isInWindow(c1, c2));

        CPPUNIT_ASSERT(RS_Vector(2.5,5.5).isInWindow(c1, c2));
        
        CPPUNIT_ASSERT(RS_Vector(2,3).isInWindow(c1, c2));
        CPPUNIT_ASSERT(RS_Vector(2,7).isInWindow(c1, c2));
        CPPUNIT_ASSERT(RS_Vector(5,3).isInWindow(c1, c2));
        CPPUNIT_ASSERT(RS_Vector(5,7).isInWindow(c1, c2));
        
        CPPUNIT_ASSERT(RS_Vector(2,3).isInWindow(c2, c1));
        CPPUNIT_ASSERT(RS_Vector(2,7).isInWindow(c2, c1));
        CPPUNIT_ASSERT(RS_Vector(5,3).isInWindow(c2, c1));
        CPPUNIT_ASSERT(RS_Vector(5,7).isInWindow(c2, c1));
        
        CPPUNIT_ASSERT(c1.isInWindow(c1, c1));
        CPPUNIT_ASSERT(!c2.isInWindow(c1, c1));
        
        c1 = RS_Vector(-2,-3);
        c2 = RS_Vector(5,7);
        
        CPPUNIT_ASSERT(RS_Vector(-1,-1).isInWindow(c2, c1));
        CPPUNIT_ASSERT(RS_Vector(-2,-3).isInWindow(c2, c1));
        CPPUNIT_ASSERT(RS_Vector(-1,1).isInWindow(c2, c1));
        CPPUNIT_ASSERT(RS_Vector(1,1).isInWindow(c2, c1));
        CPPUNIT_ASSERT(RS_Vector(1,-1).isInWindow(c2, c1));
        CPPUNIT_ASSERT(RS_Vector(1,1).isInWindow(c2, c1));
    }
    
    void testRotate() {
        RS_Vector v1(2,0);

        for (int i=1; i<100; ++i) {
            v1.rotate(2*M_PI/100);
            CPPUNIT_ASSERT(v1.magnitude()-2 < TESTTOL);
            CPPUNIT_ASSERT(v1.angle()-i*(2*M_PI/100) < TESTTOL);
        }
    }
    
    void testIsoProject() {
        RS_Vector v1(0,0);
        v1.isoProject(RS2::IsoTop);
        CPPUNIT_ASSERT(v1.x < TESTTOL);
        CPPUNIT_ASSERT(v1.y < TESTTOL);

        v1.x = 10.0;
        v1.y = 0.0;
        v1.isoProject(RS2::IsoSide);
        CPPUNIT_ASSERT(RS_Vector(7.0710678119, 4.0824829046).distanceTo(v1) < TESTTOL);
        v1.x = 10.0;
        v1.y = 5.0;
        v1.isoProject(RS2::IsoSide);
        CPPUNIT_ASSERT(RS_Vector(7.071067811865, 8.164965809277).distanceTo(v1) < TESTTOL);
        
        v1.x = 10.0;
        v1.y = 0.0;
        v1.isoProject(RS2::IsoFront);
        CPPUNIT_ASSERT_MESSAGE(
            (const char*)RS_String("%1/%2").arg(v1.x, 0, 'g', 12).arg(v1.y, 0, 'g', 12).toLatin1(), 
            RS_Vector(7.071067811865, -4.082482904639).distanceTo(v1) < TESTTOL);
        v1.x = 10.0;
        v1.y = 5.0;
        v1.isoProject(RS2::IsoFront);
        CPPUNIT_ASSERT_MESSAGE(
            (const char*)RS_String("%1/%2").arg(v1.x, 0, 'g', 12).arg(v1.y, 0, 'g', 12).toLatin1(), 
            RS_Vector(7.071067811865, 0.0).distanceTo(v1) < TESTTOL);
        
        v1.x = 10.0;
        v1.y = 0.0;
        v1.isoProject(RS2::IsoTop);
        CPPUNIT_ASSERT_MESSAGE(
            (const char*)RS_String("%1/%2").arg(v1.x, 0, 'g', 12).arg(v1.y, 0, 'g', 12).toLatin1(), 
            RS_Vector(7.0710678119, -4.0824829046).distanceTo(v1) < TESTTOL);
        v1.x = 10.0;
        v1.y = 5.0;
        v1.isoProject(RS2::IsoTop);
        CPPUNIT_ASSERT_MESSAGE(
            (const char*)RS_String("%1/%2").arg(v1.x, 0, 'g', 12).arg(v1.y, 0, 'g', 12).toLatin1(), 
            RS_Vector(10.6066017178, -2.04124145232).distanceTo(v1) < TESTTOL);
    }
};

#endif
