#include <stdio.h>
#include "rs_test.h"
#include "rs_ellipse.h"

#define TESTTOL 1.0e-10

class RS_EllipseTest : public RS_Test  {
    CPPUNIT_TEST_SUITE( RS_EllipseTest );
    CPPUNIT_TEST( testGetStartpoint );
    CPPUNIT_TEST( testGetAngleLength );
    CPPUNIT_TEST( testGetNearestPointOnEntity001 );
    CPPUNIT_TEST( testGetNearestPointOnEntity002 );
    CPPUNIT_TEST( testRendering );
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_EllipseTest");
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_EllipseTest: OK");
    }
    
    void testGetStartpoint() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testGetStartpoint");
        
        RS_Ellipse e1(NULL, 
            RS_EllipseData(RS_Vector(125, 125), 
                           RS_Vector(100, 0),
                           0.5,
                           0,2*M_PI,
                           false));
        CPPUNIT_ASSERT( RS_Vector(125+100,125).distanceTo(e1.getStartpoint())<TESTTOL );
        CPPUNIT_ASSERT( RS_Vector(125+100,125).distanceTo(e1.getEndpoint())<TESTTOL );
        
        RS_Ellipse e2(NULL, 
            RS_EllipseData(RS_Vector(125, 125), 
                           RS_Vector(100, 0),
                           0.5,
                           0,M_PI,
                           false));
        CPPUNIT_ASSERT( RS_Vector(125+100,125).distanceTo(e2.getStartpoint())<TESTTOL );
        CPPUNIT_ASSERT( RS_Vector(125-100,125).distanceTo(e2.getEndpoint())<TESTTOL );
        
        RS_Ellipse e3(NULL, 
            RS_EllipseData(RS_Vector(125, 125), 
                           RS_Vector(100, 0),
                           0.5,
                           0,M_PI/2.0,
                           false));
        CPPUNIT_ASSERT( RS_Vector(125+100,125).distanceTo(e3.getStartpoint())<TESTTOL );
        CPPUNIT_ASSERT( RS_Vector(125,125+50).distanceTo(e3.getEndpoint())<TESTTOL );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testGetStartpoint");
    }
    
    void testGetAngleLength() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testGetAngleLength");

        RS_Ellipse e1(NULL, 
            RS_EllipseData(RS_Vector(125, 125), 
                           RS_Vector(100, 0),
                           0.5,
                           0,M_PI/2,
                           false));
        CPPUNIT_ASSERT( fabs(e1.getAngleLength()-M_PI/2)<TESTTOL );
        
        RS_Ellipse e2(NULL, 
            RS_EllipseData(RS_Vector(125, 125), 
                           RS_Vector(100, 0),
                           0.5,
                           0,M_PI/2,
                           true));
        CPPUNIT_ASSERT( fabs(e2.getAngleLength()-3*M_PI/2)<TESTTOL );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testGetAngleLength: OK");
    }

    /**
     * Tests getNearestPointOnEntity with full ellipses.
     */
    void testGetNearestPointOnEntity001() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testGetNearestPointOnEntity001");
        
        double dist;
        RS_Entity* ent;
        RS_Vector res;
        
        RS_Ellipse e1(NULL, 
            RS_EllipseData(RS_Vector(125, 125), 
                           RS_Vector(100, 0),
                           0.5,
                           0,2*M_PI,
                           false));

        double x, y;
        for (x=125+10; x<250.0; x+=1.0) {
            res = e1.getNearestPointOnEntity(RS_Vector(x,125), true, &dist, &ent);
            CPPUNIT_ASSERT( res.valid );
            CPPUNIT_ASSERT( ent==&e1 );
            CPPUNIT_ASSERT( res.distanceTo(RS_Vector(125+100,125))<1.0e-6 );
            CPPUNIT_ASSERT( fabs(dist-fabs(125+100-x))<1.0e-6 );
        }
        
        for (y=125+10; y<250.0; y+=1.0) {
            res = e1.getNearestPointOnEntity(RS_Vector(125,y), true, &dist, &ent);
            CPPUNIT_ASSERT( res.valid );
            CPPUNIT_ASSERT( ent==&e1 );
            CPPUNIT_ASSERT( res.distanceTo(RS_Vector(125,125+50))<1.0e-6 );
            CPPUNIT_ASSERT( fabs(dist-fabs(125+50-y))<1.0e-6 );
        }
        
        for (x=125-10; x>0.0; x-=1.0) {
            res = e1.getNearestPointOnEntity(RS_Vector(x,125), true, &dist, &ent);
            CPPUNIT_ASSERT( res.valid );
            CPPUNIT_ASSERT( ent==&e1 );
            CPPUNIT_ASSERT( res.distanceTo(RS_Vector(125-100,125))<1.0e-6 );
            CPPUNIT_ASSERT( fabs(dist-fabs(125-100-x))<1.0e-6 );
        }
        
        for (y=125-10; y>0.0; y-=1.0) {
            res = e1.getNearestPointOnEntity(RS_Vector(125,y), true, &dist, &ent);
            CPPUNIT_ASSERT( res.valid );
            CPPUNIT_ASSERT( ent==&e1 );
            CPPUNIT_ASSERT( res.distanceTo(RS_Vector(125,125-50))<1.0e-6 );
            CPPUNIT_ASSERT( fabs(dist-fabs(125-50-y))<1.0e-6 );
        }
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testGetNearestPointOnEntity001: OK");
    }
    
    void testGetNearestPointOnEntity002() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testGetNearestPointOnEntity002");
        
        double dist;
        RS_Entity* ent;
        RS_Vector res;
        
        RS_Ellipse e1(NULL, 
            RS_EllipseData(RS_Vector(125, 125), 
                           RS_Vector(100, 0),
                           0.5,
                           0,M_PI/2.0,
                           false));

        double x, y, a;
        for (x=125+10; x<250.0; x+=1.0) {
            res = e1.getNearestPointOnEntity(RS_Vector(x,125), true, &dist, &ent);
            CPPUNIT_ASSERT( res.valid );
            CPPUNIT_ASSERT( ent==&e1 );
            CPPUNIT_ASSERT( res.distanceTo(RS_Vector(125+100,125))<1.0e-6 );
            CPPUNIT_ASSERT( fabs(dist-fabs(125+100-x))<1.0e-6 );
        }
        
        for (y=125+10; y<250.0; y+=1.0) {
            res = e1.getNearestPointOnEntity(RS_Vector(125,y), true, &dist, &ent);
            CPPUNIT_ASSERT( res.valid );
            CPPUNIT_ASSERT( ent==&e1 );
            CPPUNIT_ASSERT( res.distanceTo(RS_Vector(125,125+50))<1.0e-6 );
            CPPUNIT_ASSERT( fabs(dist-fabs(125+50-y))<1.0e-6 );
        }
        
        for (x=125-10; x>0.0; x-=1.0) {
            res = e1.getNearestPointOnEntity(RS_Vector(x,125), true, &dist, &ent);
            CPPUNIT_ASSERT( !res.valid );
        }
        
        for (y=125-10; y>0.0; y-=1.0) {
            res = e1.getNearestPointOnEntity(RS_Vector(125,y), true, &dist, &ent);
            CPPUNIT_ASSERT( !res.valid );
        }
        
        for (a=0.0; a<M_PI/2.0; a+=0.1) {
            RS_Vector p;
            p.setPolar(105.0, a);
            p.move(RS_Vector(125,125));
            RS_DEBUG->print(RS_Debug::D_TESTING, "testGetNearestPointOnEntity002: a: %f", a);
            res = e1.getNearestPointOnEntity(p, true, &dist, &ent);
            CPPUNIT_ASSERT( res.valid );
        }

        for (a=M_PI/2.0+0.1; a<2*M_PI-0.1; a+=0.1) {
            RS_Vector p;
            p.setPolar(105.0, a);
            p.move(RS_Vector(125,125));
            RS_DEBUG->print(RS_Debug::D_TESTING, "testGetNearestPointOnEntity002: a: %f", a);
            res = e1.getNearestPointOnEntity(p, true, &dist, &ent);
            CPPUNIT_ASSERT( !res.valid );
        }
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testGetNearestPointOnEntity002: OK");
    }
    
    void testRendering() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering");
        
        RS_Ellipse* ellipse;
        RS_EntityContainer ec;
        ec.addEntity(new RS_Ellipse(&ec, 
            RS_EllipseData(RS_Vector(125, 125), 
                           RS_Vector(100, 0),
                           0.5,
                           0,M_PI/2,
                           false)));
        saveAndCompare(&ec, "ellipse000");

        ec.clear();
        ec.addEntity(new RS_Ellipse(&ec, 
            RS_EllipseData(RS_Vector(125, 125), 
                           RS_Vector(100, 0),
                           0.5,
                           0,M_PI/2,
                           true)));
        saveAndCompare(&ec, "ellipse001");
        
        ec.clear();
        ellipse = new RS_Ellipse(&ec, 
            RS_EllipseData(RS_Vector(125, 125), 
                           RS_Vector(100, 0),
                           0.5,
                           0,M_PI/2,
                           false));
        ellipse->setPen(RS_Pen(RS_Color(0,0,0),
                               RS2::Width00,
                               RS2::DashLine));
        ec.addEntity(ellipse);
        saveAndCompare(&ec, "ellipse002");
        
        ec.clear();
        ellipse = new RS_Ellipse(&ec, 
            RS_EllipseData(RS_Vector(125, 125), 
                           RS_Vector(100, 0),
                           0.5,
                           0,M_PI/2,
                           true));
        ellipse->setPen(RS_Pen(RS_Color(0,0,0),
                               RS2::Width00,
                               RS2::DashLine));
        ec.addEntity(ellipse);
        saveAndCompare(&ec, "ellipse003");
        
        /*
        ec.clear();
        ellipse = new RS_Ellipse(&ec, 
            RS_EllipseData(RS_Vector(54.75,56.75), 
                           RS_Vector(117.25, 26.25),
                           9.09,
                           0,M_PI/2,
                           true));
        ellipse->setPen(RS_Pen(RS_Color(0,0,0),
                               RS2::Width17,
                               RS2::DashLine));
        ec.addEntity(ellipse);
        saveAndCompare(&ec, "ellipse004");
        */
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering: OK");
    }
};
