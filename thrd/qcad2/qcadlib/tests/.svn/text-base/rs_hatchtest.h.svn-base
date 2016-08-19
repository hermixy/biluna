#include <stdio.h>
#include "rs_test.h"
#include "rs_hatch.h"
#include "rs_modification.h"

class RS_HatchTest : public RS_Test  {
    CPPUNIT_TEST_SUITE( RS_HatchTest );
    CPPUNIT_TEST( testRendering000 );
    CPPUNIT_TEST( testRendering001 );
    CPPUNIT_TEST( testRendering002 );
    CPPUNIT_TEST( testRendering003 );
    CPPUNIT_TEST( testRendering004 );
    CPPUNIT_TEST( testRendering005 );
    CPPUNIT_TEST( testRendering006 );
    CPPUNIT_TEST( testRendering007 );
    CPPUNIT_TEST( testRendering008 );
    CPPUNIT_TEST( testRendering009 );
    CPPUNIT_TEST( testRendering010 );
#ifdef RS_ALGO
    CPPUNIT_TEST( testTriangulation );
#endif
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_HatchTest");
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_HatchTest: OK");
    }
    
    void testRendering000() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering000");
        
        RS_EntityContainer ec;
        
        RS_Hatch* hatch = new RS_Hatch(&ec, 
            RS_HatchData(true, 1.0, 0.0, "solid"));
        hatch->setPen(
            RS_Pen(RS_Color(255,0,0), RS2::Width01, RS2::SolidLine));
        
        RS_EntityContainer* loop = new RS_EntityContainer(hatch);
        loop->addEntity(new RS_Line(loop, 
            RS_LineData(RS_Vector(10,10), RS_Vector(240,10))));
        loop->addEntity(new RS_Line(loop, 
            RS_LineData(RS_Vector(240,10), RS_Vector(125,240))));
        loop->addEntity(new RS_Line(loop, 
            RS_LineData(RS_Vector(125,240), RS_Vector(10,10))));
        hatch->addEntity(loop);
        
        ec.addEntity(hatch);
        
        saveAndCompare(&ec, "hatch000");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering000: OK");
    }
    
    void testRendering001() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering001");
        
        RS_EntityContainer ec;
        
        RS_Hatch* hatch = new RS_Hatch(&ec, 
            RS_HatchData(true, 1.0, 0.0, "solid"));
        hatch->setPen(
            RS_Pen(RS_Color(255,0,0), RS2::Width01, RS2::SolidLine));
        
        RS_EntityContainer* loop = new RS_EntityContainer(hatch);
        loop->addEntity(new RS_Line(loop, 
            RS_LineData(RS_Vector(10,10), RS_Vector(240,10))));
        loop->addEntity(new RS_Arc(loop, 
            RS_ArcData(RS_Vector(125,10), 115, 0, M_PI, false)));
        hatch->addEntity(loop);
        
        ec.addEntity(hatch);
        
        saveAndCompare(&ec, "hatch001");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering001: OK");
    }
    
    void testRendering002() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering002");
        
        RS_EntityContainer ec;
        
        RS_Hatch* hatch = new RS_Hatch(&ec, 
            RS_HatchData(true, 1.0, 0.0, "solid"));
        hatch->setPen(
            RS_Pen(RS_Color(255,0,0), RS2::Width01, RS2::SolidLine));
        
        RS_EntityContainer* loop1 = new RS_EntityContainer(hatch);
        loop1->addEntity(new RS_Line(loop1, 
            RS_LineData(RS_Vector(10,10), RS_Vector(240,10))));
        loop1->addEntity(new RS_Line(loop1, 
            RS_LineData(RS_Vector(240,10), RS_Vector(240,240))));
        loop1->addEntity(new RS_Line(loop1, 
            RS_LineData(RS_Vector(240,240), RS_Vector(10,10))));
        hatch->addEntity(loop1);
        
        RS_EntityContainer* loop2 = new RS_EntityContainer(hatch);
        loop2->addEntity(new RS_Line(loop2, 
            RS_LineData(RS_Vector(10,60), RS_Vector(190,240))));
        loop2->addEntity(new RS_Line(loop2, 
            RS_LineData(RS_Vector(190,240), RS_Vector(10,240))));
        loop2->addEntity(new RS_Line(loop2, 
            RS_LineData(RS_Vector(10,240), RS_Vector(10,60))));
        hatch->addEntity(loop2);
        
        ec.addEntity(hatch);
        
        saveAndCompare(&ec, "hatch002");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering002: OK");
    }
    
    void testRendering003() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering003");
        
        RS_EntityContainer ec;
        
        RS_Hatch* hatch = new RS_Hatch(&ec, 
            RS_HatchData(true, 1.0, 0.0, "solid"));
        hatch->setPen(
            RS_Pen(RS_Color(255,0,0), RS2::Width01, RS2::SolidLine));
        
        RS_EntityContainer* loop1 = new RS_EntityContainer(hatch);
        loop1->addEntity(new RS_Line(loop1, 
            RS_LineData(RS_Vector(10,10), RS_Vector(240,10))));
        loop1->addEntity(new RS_Line(loop1, 
            RS_LineData(RS_Vector(240,10), RS_Vector(240,240))));
        loop1->addEntity(new RS_Line(loop1, 
            RS_LineData(RS_Vector(240,240), RS_Vector(10,10))));
        hatch->addEntity(loop1);
        
        RS_EntityContainer* loop2 = new RS_EntityContainer(hatch);
        loop2->addEntity(new RS_Line(loop2, 
            RS_LineData(RS_Vector(85,35), RS_Vector(215,35))));
        loop2->addEntity(new RS_Line(loop2, 
            RS_LineData(RS_Vector(215,35), RS_Vector(215,165))));
        loop2->addEntity(new RS_Line(loop2, 
            RS_LineData(RS_Vector(215,165), RS_Vector(85,35))));
        hatch->addEntity(loop2);
        
        ec.addEntity(hatch);
        
        saveAndCompare(&ec, "hatch003");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering003: OK");
    }
    
    void testRendering004() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering004");
        
        RS_EntityContainer ec;
        
        RS_Hatch* hatch = new RS_Hatch(&ec, 
            RS_HatchData(true, 1.0, 0.0, "solid"));
        hatch->setPen(
            RS_Pen(RS_Color(255,0,0), RS2::Width01, RS2::SolidLine));
        
        RS_EntityContainer* loop = new RS_EntityContainer(hatch);
        loop->addEntity(new RS_Line(loop, 
            RS_LineData(RS_Vector(240,240), RS_Vector(10,240))));
        loop->addEntity(new RS_Arc(loop, 
            RS_ArcData(RS_Vector(125,240), 115, 0, M_PI, true)));
        hatch->addEntity(loop);
        
        ec.addEntity(hatch);
        
        saveAndCompare(&ec, "hatch004");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering004: OK");
    }
    
    void testRendering005() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering005");
        
        RS_EntityContainer ec;
        
        RS_Hatch* hatch = new RS_Hatch(&ec, 
            RS_HatchData(true, 1.0, 0.0, "solid"));
        hatch->setPen(
            RS_Pen(RS_Color(255,0,0), RS2::Width01, RS2::SolidLine));
        
        RS_EntityContainer* loop1 = new RS_EntityContainer(hatch);
        loop1->addEntity(new RS_Line(loop1, 
            RS_LineData(RS_Vector(10,10), RS_Vector(240,10))));
        loop1->addEntity(new RS_Line(loop1, 
            RS_LineData(RS_Vector(240,10), RS_Vector(240,240))));
        loop1->addEntity(new RS_Line(loop1, 
            RS_LineData(RS_Vector(240,240), RS_Vector(10,240))));
        loop1->addEntity(new RS_Line(loop1, 
            RS_LineData(RS_Vector(10,240), RS_Vector(10,10))));
        hatch->addEntity(loop1);
        
        RS_EntityContainer* loop2 = new RS_EntityContainer(hatch);
        loop2->addEntity(new RS_Circle(loop2, 
            RS_CircleData(RS_Vector(125,125), 50)));
        hatch->addEntity(loop2);
        
        ec.addEntity(hatch);
        
        saveAndCompare(&ec, "hatch005");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering005: OK");
    }
    
    void testRendering006() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering006");
        
        RS_EntityContainer ec;
        
        RS_Hatch* hatch = new RS_Hatch(&ec, 
            RS_HatchData(true, 1.0, 0.0, "solid"));
        hatch->setPen(
            RS_Pen(RS_Color(255,0,0), RS2::Width01, RS2::SolidLine));
        
        RS_EntityContainer* loop1 = new RS_EntityContainer(hatch);
        loop1->addEntity(new RS_Line(loop1, 
            RS_LineData(RS_Vector(44.3845,50), RS_Vector(30,50))));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(44.3845,30), 20, 62.734/ARAD, 90/ARAD, false)));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(65,70), 25, 242.734/ARAD, 297.266/ARAD, false)));
        loop1->addEntity(new RS_Line(loop1, 
            RS_LineData(RS_Vector(85.6155,50), RS_Vector(100,50))));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(85.6155,30), 20, 90/ARAD, 117.266/ARAD, false)));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(30,30), 20, 90/ARAD, 180/ARAD, false)));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(100,30), 20, 0/ARAD, 90/ARAD, false)));
        
        loop1->addEntity(new RS_Line(loop1, 
            RS_LineData(RS_Vector(44.3845,10), RS_Vector(30,10))));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(44.3845,30), 20, 270/ARAD, 297.266/ARAD, false)));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(65,-10), 25, 62.734/ARAD, 117.266/ARAD, false)));
        loop1->addEntity(new RS_Line(loop1, 
            RS_LineData(RS_Vector(85.6155,10), RS_Vector(100,10))));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(85.6155,30), 20, 242.734/ARAD, 270/ARAD, false)));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(30,30), 20, 180/ARAD, 270/ARAD, false)));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(100,30), 20, 270/ARAD, 0/ARAD, false)));
        hatch->addEntity(loop1);
        
        /*
        RS_EntityContainer* loop2 = new RS_EntityContainer(hatch);
        loop2->addEntity(new RS_Circle(hatch, 
            RS_CircleData(RS_Vector(125,125), 50)));
        hatch->addEntity(loop2);
        */
        
        ec.addEntity(hatch);
        
        saveAndCompare(&ec, "hatch006");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering006: OK");
    }
    
    void testRendering007() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering007");
        
        RS_EntityContainer ec;
        
        RS_Hatch* hatch = new RS_Hatch(&ec, 
            RS_HatchData(true, 1.0, 0.0, "solid"));
        hatch->setPen(
            RS_Pen(RS_Color(255,0,0), RS2::Width01, RS2::SolidLine));
        
        RS_EntityContainer* loop = new RS_EntityContainer(hatch);
        loop->addEntity(new RS_Line(loop, 
            RS_LineData(RS_Vector(125,10), RS_Vector(125,240))));
        loop->addEntity(new RS_Arc(loop, 
            RS_ArcData(RS_Vector(125,125), 115, M_PI/2, M_PI/2*3, false)));
        hatch->addEntity(loop);
        
        ec.addEntity(hatch);
        
        saveAndCompare(&ec, "hatch007");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering007: OK");
    }
    
    void testRendering008() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering008");
        
        RS_EntityContainer ec;
        
        RS_Hatch* hatch = new RS_Hatch(&ec, 
            RS_HatchData(true, 1.0, 0.0, "solid"));
        hatch->setPen(
            RS_Pen(RS_Color(255,0,0), RS2::Width01, RS2::SolidLine));
        
        RS_EntityContainer* loop = new RS_EntityContainer(hatch);
        loop->addEntity(new RS_Line(loop, 
            RS_LineData(RS_Vector(125,10), RS_Vector(125,240))));
        loop->addEntity(new RS_Arc(loop, 
            RS_ArcData(RS_Vector(125,125), 115, M_PI/2, M_PI/2*3, true)));
        hatch->addEntity(loop);
        
        ec.addEntity(hatch);
        
        saveAndCompare(&ec, "hatch008");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering008: OK");
    }
    
    void testRendering009() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering009");
        
        RS_EntityContainer ec;
        
        RS_Hatch* hatch = new RS_Hatch(&ec, 
            RS_HatchData(true, 1.0, 0.0, "solid"));
        hatch->setPen(
            RS_Pen(RS_Color(255,0,0), RS2::Width01, RS2::SolidLine));
        
        RS_EntityContainer* loop1 = new RS_EntityContainer(hatch);
        loop1->addEntity(new RS_Line(loop1, 
            RS_LineData(RS_Vector(44.3845,50), RS_Vector(30,50))));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(44.3845,30), 20, 62.734/ARAD, 90/ARAD, false)));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(65,70), 25, 242.734/ARAD, 297.266/ARAD, false)));
        loop1->addEntity(new RS_Line(loop1, 
            RS_LineData(RS_Vector(85.6155,50), RS_Vector(100,50))));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(85.6155,30), 20, 90/ARAD, 117.266/ARAD, false)));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(30,30), 20, 90/ARAD, 180/ARAD, false)));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(100,30), 20, 0/ARAD, 90/ARAD, false)));
        
        loop1->addEntity(new RS_Line(loop1, 
            RS_LineData(RS_Vector(44.3845,10), RS_Vector(30,10))));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(44.3845,30), 20, 270/ARAD, 297.266/ARAD, false)));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(65,-10), 25, 62.734/ARAD, 117.266/ARAD, false)));
        loop1->addEntity(new RS_Line(loop1, 
            RS_LineData(RS_Vector(85.6155,10), RS_Vector(100,10))));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(85.6155,30), 20, 242.734/ARAD, 270/ARAD, false)));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(30,30), 20, 180/ARAD, 270/ARAD, false)));
        loop1->addEntity(new RS_Arc(loop1, 
            RS_ArcData(RS_Vector(100,30), 20, 270/ARAD, 0/ARAD, false)));
        hatch->addEntity(loop1);

        RS_EntityContainer* loop2 = new RS_EntityContainer(hatch);
        loop2->addEntity(new RS_Arc(loop2, 
            RS_ArcData(RS_Vector(30,30), 10, 0/ARAD, 180/ARAD, false)));
        loop2->addEntity(new RS_Arc(loop2, 
            RS_ArcData(RS_Vector(30,30), 10, 180/ARAD, 0/ARAD, false)));
        hatch->addEntity(loop2);

        RS_EntityContainer* loop3 = new RS_EntityContainer(hatch);
        loop3->addEntity(new RS_Arc(loop3, 
            RS_ArcData(RS_Vector(100,30), 10, 360/ARAD, 180/ARAD, false)));
        loop3->addEntity(new RS_Arc(loop3, 
            RS_ArcData(RS_Vector(100,30), 10, 180/ARAD, 0/ARAD, false)));
        hatch->addEntity(loop3);
        
        ec.addEntity(hatch);
        
        saveAndCompare(&ec, "hatch009");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering009: OK");
    }
    
    void testRendering010() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering010");
        
        RS_EntityContainer ec;
        
        RS_Hatch* hatch = new RS_Hatch(&ec, 
            RS_HatchData(true, 1.0, 1.0, "solid"));
        hatch->setPen(
            RS_Pen(RS_Color(255,0,0), RS2::Width01, RS2::SolidLine));
        
        RS_Polyline* loop1 = new RS_Polyline(hatch);
        loop1->addVertex(RS_Vector(10,10));
        loop1->addVertex(RS_Vector(90,10));
        loop1->addVertex(RS_Vector(90,90));
        loop1->addVertex(RS_Vector(10,90));
        loop1->addVertex(RS_Vector(10,10));
        hatch->addEntity(loop1);
        
        /*
        RS_EntityContainer* loop1 = new RS_EntityContainer(hatch);
        loop1->addEntity(new RS_Line(loop1, 
            RS_LineData(RS_Vector(10,10), RS_Vector(90,10))));
        loop1->addEntity(new RS_Line(loop1,
            RS_LineData(RS_Vector(90,10), RS_Vector(90,90))));
        loop1->addEntity(new RS_Line(loop1,
            RS_LineData(RS_Vector(90,90), RS_Vector(10,90))));
        loop1->addEntity(new RS_Line(loop1,
            RS_LineData(RS_Vector(10,90), RS_Vector(10,10))));
        hatch->addEntity(loop1);
        */
        
        ec.addEntity(hatch);
        
        saveAndCompare(&ec, "hatch010");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering010: OK");
    }
    
#ifdef RS_ALGO
    void testTriangulation() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testTriangulation");
        
        RS_EntityContainer ec;
        
        RS_Hatch* hatch = new RS_Hatch(&ec, 
            RS_HatchData(true, 1.0, 1.0, "solid"));
        hatch->setPen(
            RS_Pen(RS_Color(255,0,0), RS2::Width01, RS2::SolidLine));
        
        RS_EntityContainer* loop1 = new RS_EntityContainer(hatch);
        loop1->addEntity(new RS_Line(loop1, RS_LineData(RS_Vector(10,10), RS_Vector(90,10))));
        loop1->addEntity(new RS_Line(loop1, RS_LineData(RS_Vector(90,10), RS_Vector(90,90))));
        loop1->addEntity(new RS_Line(loop1, RS_LineData(RS_Vector(90,90), RS_Vector(10,90))));
        loop1->addEntity(new RS_Line(loop1, RS_LineData(RS_Vector(10,90), RS_Vector(10,10))));
        hatch->addEntity(loop1);
        hatch->setSelected(true);
        
        ec.addEntity(hatch);

        RS_Modification modification(ec, NULL, false);
        modification.triangulate();
            
        //CPPUNIT_ASSERT(ec.count()==2);

        for (RS_Entity* e=ec.firstEntity(); e!=NULL; e=ec.nextEntity()) {
            CPPUNIT_ASSERT(e->rtti()==RS2::EntityHatch);
        }
        
        //saveAndCompare(&ec, "hatch010");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testTriangulation: OK");
    }
#endif
};
