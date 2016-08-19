#include <stdio.h>
#include "rs_test.h"
#include "rs_math.h"
#include "rs_modification.h"

class RS_ModificationDivide2Test : public RS_Test  {
    CPPUNIT_TEST_SUITE( RS_ModificationDivide2Test );

    // divide2 on lines:
    CPPUNIT_TEST( testDivide2001 );
    CPPUNIT_TEST( testDivide2002 );
    CPPUNIT_TEST( testDivide2003 );
    CPPUNIT_TEST( testDivide2004 );
    
    // divide2 on arcs:
    CPPUNIT_TEST( testDivide2011 );
    CPPUNIT_TEST( testDivide2012 );
    CPPUNIT_TEST( testDivide2013 );
    CPPUNIT_TEST( testDivide2014 );
    
    // divide2 on arcs with disappearing segments:
    CPPUNIT_TEST( testDivide2015 );
    CPPUNIT_TEST( testDivide2016 );
    CPPUNIT_TEST( testDivide2017 );
    CPPUNIT_TEST( testDivide2018 );

    CPPUNIT_TEST( testDivide2019 );
    CPPUNIT_TEST( testDivide2020 );
    CPPUNIT_TEST( testDivide2021 );
    CPPUNIT_TEST( testDivide2022 );
    
    // divide2 on circles:
    CPPUNIT_TEST( testDivide2030 );
    CPPUNIT_TEST( testDivide2031 );
    CPPUNIT_TEST( testDivide2032 );
    CPPUNIT_TEST( testDivide2033 );
    
    CPPUNIT_TEST( testDivide2034 );
    CPPUNIT_TEST( testDivide2035 );
    CPPUNIT_TEST( testDivide2036 );
    CPPUNIT_TEST( testDivide2037 );
    
    CPPUNIT_TEST( testDivide2038 );
    CPPUNIT_TEST( testDivide2039 );

    // divide2 with limiting entities / lines:
    CPPUNIT_TEST( testDivide2050 );
    CPPUNIT_TEST( testDivide2051 );
    CPPUNIT_TEST( testDivide2052 );
    CPPUNIT_TEST( testDivide2053 );

    // divide2 with limiting entities / arcs:
    CPPUNIT_TEST( testDivide2060 );
    CPPUNIT_TEST( testDivide2061 );
    
    // divide2 with limiting entities / circles:
    CPPUNIT_TEST( testDivide2070 );
    CPPUNIT_TEST( testDivide2071 );
    
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_ModificationDivide2Test");
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_ModificationDivide2Test: OK");
    }
    
    /**
     * divide2: line / normal case
     */
    void testDivide2001() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2001");
        
        RS_Modification modification(ec);
        
        RS_Line* line = new RS_Line(&ec, 
            RS_LineData(RS_Vector(10,10), RS_Vector(240,10)));
        ec.addEntity(line);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        modification.divide2(RS_Vector(110,10), RS_Vector(140,10), line, RS_Vector(false));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==2 );
        
        saveAndCompare(&ec, "modification_divide2001");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2001: OK");
    }
    
    /**
     * divide2: line / start segment disappears
     */
    void testDivide2002() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2002");
        
        RS_Modification modification(ec);
        
        RS_Line* line = new RS_Line(&ec, 
            RS_LineData(RS_Vector(10,10), RS_Vector(240,10)));
        ec.addEntity(line);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        modification.divide2(RS_Vector(10,10), RS_Vector(125,10), line, RS_Vector(false));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==1 );
        
        saveAndCompare(&ec, "modification_divide2002");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2002: OK");
    }
    
    /**
     * divide2: line / end segment disappears
     */
    void testDivide2003() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2003");
        
        RS_Modification modification(ec);
        
        RS_Line* line = new RS_Line(&ec, 
            RS_LineData(RS_Vector(10,10), RS_Vector(240,10)));
        ec.addEntity(line);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        modification.divide2(RS_Vector(125,10), RS_Vector(240,10), line, RS_Vector(false));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==1 );
        
        saveAndCompare(&ec, "modification_divide2003");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2003: OK");
    }
    
    /**
     * divide2: line / whole line disappears
     */
    void testDivide2004() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2004");
        
        RS_Modification modification(ec);
        
        RS_Line* line = new RS_Line(&ec, 
            RS_LineData(RS_Vector(10,10), RS_Vector(240,10)));
        ec.addEntity(line);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        modification.divide2(RS_Vector(10,10), RS_Vector(240,10), line, RS_Vector(false));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==0 );
        
        saveAndCompare(&ec, "modification_divide2004");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2004: OK");
    }
    
    /**
     * divide2: arc / normal case
     */
    void testDivide2011() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2011");
        
        RS_Modification modification(ec);
        
        RS_Arc* arc = new RS_Arc(&ec, 
            RS_ArcData(RS_Vector(125,125), 
                100.0, 
                RS_Math::deg2rad(45.0), 
                RS_Math::deg2rad(135.0), 
                false));
        ec.addEntity(arc);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(85.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(95.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint1, cutPoint2, arc, RS_Vector(false));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==2 );
        
        saveAndCompare(&ec, "modification_divide2011");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2011: OK");
    }
    
    /**
     * divide2: arc / normal case 2
     */
    void testDivide2012() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2012");
        
        RS_Modification modification(ec);
        
        RS_Arc* arc = new RS_Arc(&ec, 
            RS_ArcData(RS_Vector(125,125), 
                100.0, 
                RS_Math::deg2rad(45.0), 
                RS_Math::deg2rad(135.0), 
                false));
        ec.addEntity(arc);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(85.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(95.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint2, cutPoint1, arc, RS_Vector(false));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==2 );
        
        saveAndCompare(&ec, "modification_divide2012");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2012: OK");
    }
    
    /**
     * divide2: arc reversed / normal case
     */
    void testDivide2013() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2013");
        
        RS_Modification modification(ec);
        
        RS_Arc* arc = new RS_Arc(&ec, 
            RS_ArcData(RS_Vector(125,125), 
                100.0, 
                RS_Math::deg2rad(135.0), 
                RS_Math::deg2rad(45.0), 
                true));
        ec.addEntity(arc);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(85.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(95.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint1, cutPoint2, arc, RS_Vector(false));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==2 );
        
        saveAndCompare(&ec, "modification_divide2013");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2013: OK");
    }
    
    /**
     * divide2: arc reversed / normal case 2
     */
    void testDivide2014() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2014");
        
        RS_Modification modification(ec);
        
        RS_Arc* arc = new RS_Arc(&ec, 
            RS_ArcData(RS_Vector(125,125), 
                100.0, 
                RS_Math::deg2rad(135.0), RS_Math::deg2rad(45.0), 
                true));
        ec.addEntity(arc);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(85.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(95.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint2, cutPoint1, arc, RS_Vector(false));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==2 );
        
        saveAndCompare(&ec, "modification_divide2014");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2014: OK");
    }
    
    /**
     * divide2: arc / end segment disappears
     */
    void testDivide2015() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2015");
        
        RS_Modification modification(ec);
        
        RS_Arc* arc = new RS_Arc(&ec, 
            RS_ArcData(RS_Vector(125,125), 
                100.0, 
                RS_Math::deg2rad(45.0), 
                RS_Math::deg2rad(135.0), 
                false));
        ec.addEntity(arc);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(85.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(135.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint1, cutPoint2, arc, RS_Vector(false));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==1 );
        
        saveAndCompare(&ec, "modification_divide2015");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2015: OK");
    }
    
    /**
     * divide2: arc / end segment disappears 2
     */
    void testDivide2016() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2016");
        
        RS_Modification modification(ec);
        
        RS_Arc* arc = new RS_Arc(&ec, 
            RS_ArcData(RS_Vector(125,125), 
                100.0, 
                RS_Math::deg2rad(45.0), 
                RS_Math::deg2rad(135.0), 
                false));
        ec.addEntity(arc);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(85.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(135.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint2, cutPoint1, arc, RS_Vector(false));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==1 );
        
        saveAndCompare(&ec, "modification_divide2016");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2016: OK");
    }
    
    /**
     * divide2: arc reversed / end segment disappears
     */
    void testDivide2017() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2017");
        
        RS_Modification modification(ec);
        
        RS_Arc* arc = new RS_Arc(&ec, 
            RS_ArcData(RS_Vector(125,125), 
                100.0, 
                RS_Math::deg2rad(135.0), 
                RS_Math::deg2rad(45.0), 
                true));
        ec.addEntity(arc);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(85.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(135.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint1, cutPoint2, arc, RS_Vector(false));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==1 );
        
        saveAndCompare(&ec, "modification_divide2017");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2017: OK");
    }
    
    /**
     * divide2: arc reversed / end segment disappears 2
     */
    void testDivide2018() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2018");
        
        RS_Modification modification(ec);
        
        RS_Arc* arc = new RS_Arc(&ec, 
            RS_ArcData(RS_Vector(125,125), 
                100.0, 
                RS_Math::deg2rad(135.0), 
                RS_Math::deg2rad(45.0), 
                true));
        ec.addEntity(arc);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(85.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(135.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint2, cutPoint1, arc, RS_Vector(false));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==1 );
        
        saveAndCompare(&ec, "modification_divide2018");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2018: OK");
    }
    
    /**
     * divide2: arc / start segment disappears
     */
    void testDivide2019() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2019");
        
        RS_Modification modification(ec);
        
        RS_Arc* arc = new RS_Arc(&ec, 
            RS_ArcData(RS_Vector(125,125), 
                100.0, 
                RS_Math::deg2rad(45.0), 
                RS_Math::deg2rad(135.0), 
                false));
        ec.addEntity(arc);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(45.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(95.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint1, cutPoint2, arc, RS_Vector(false));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==1 );
        
        saveAndCompare(&ec, "modification_divide2019");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2019: OK");
    }
    
    /**
     * divide2: arc / start segment disappears 2
     */
    void testDivide2020() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2020");
        
        RS_Modification modification(ec);
        
        RS_Arc* arc = new RS_Arc(&ec, 
            RS_ArcData(RS_Vector(125,125), 
                100.0, 
                RS_Math::deg2rad(45.0), 
                RS_Math::deg2rad(135.0), 
                false));
        ec.addEntity(arc);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(45.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(95.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint2, cutPoint1, arc, RS_Vector(false));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==1 );
        
        saveAndCompare(&ec, "modification_divide2020");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2020: OK");
    }
    
    /**
     * divide2: arc reversed / start segment disappears
     */
    void testDivide2021() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2021");
        
        RS_Modification modification(ec);
        
        RS_Arc* arc = new RS_Arc(&ec, 
            RS_ArcData(RS_Vector(125,125), 
                100.0, 
                RS_Math::deg2rad(135.0), 
                RS_Math::deg2rad(45.0), 
                true));
        ec.addEntity(arc);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(45.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(95.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint1, cutPoint2, arc, RS_Vector(false));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==1 );
        
        saveAndCompare(&ec, "modification_divide2021");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2021: OK");
    }
    
    /**
     * divide2: arc reversed / start segment disappears 2
     */
    void testDivide2022() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2022");
        
        RS_Modification modification(ec);
        
        RS_Arc* arc = new RS_Arc(&ec, 
            RS_ArcData(RS_Vector(125,125), 
                100.0, 
                RS_Math::deg2rad(135.0), 
                RS_Math::deg2rad(45.0), 
                true));
        ec.addEntity(arc);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(45.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(95.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint2, cutPoint1, arc, RS_Vector(false));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==1 );
        
        saveAndCompare(&ec, "modification_divide2022");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2022: OK");
    }
    
    /**
     * divide2: circle / normal case
     */
    void testDivide2030() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2030");
        
        RS_Modification modification(ec);
        
        RS_Circle* circle = new RS_Circle(&ec, 
            RS_CircleData(RS_Vector(125,125), 
                100.0));
        ec.addEntity(circle);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(85.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(95.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint1, cutPoint2, circle, RS_Vector(125.0, 230.0));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==1 );
        
        saveAndCompare(&ec, "modification_divide2030");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2030: OK");
    }
    
    /**
     * divide2: circle / normal case 2
     */
    void testDivide2031() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2031");
        
        RS_Modification modification(ec);
        
        RS_Circle* circle = new RS_Circle(&ec, 
            RS_CircleData(RS_Vector(125,125), 
                100.0));
        ec.addEntity(circle);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(85.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(95.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint2, cutPoint1, circle, RS_Vector(125.0, 230.0));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==1 );
        
        saveAndCompare(&ec, "modification_divide2031");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2031: OK");
    }
    
    /**
     * divide2: circle / normal case / cut larger segment
     */
    void testDivide2032() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2032");
        
        RS_Modification modification(ec);
        
        RS_Circle* circle = new RS_Circle(&ec, 
            RS_CircleData(RS_Vector(125,125), 
                100.0));
        ec.addEntity(circle);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(85.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(95.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint1, cutPoint2, circle, RS_Vector(125.0, 10.0));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==1 );
        
        saveAndCompare(&ec, "modification_divide2032");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2032: OK");
    }
    
    /**
     * divide2: circle / normal case 2 / cut larger segment
     */
    void testDivide2033() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2033");
        
        RS_Modification modification(ec);
        
        RS_Circle* circle = new RS_Circle(&ec, 
            RS_CircleData(RS_Vector(125,125), 
                100.0));
        ec.addEntity(circle);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(85.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(95.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint2, cutPoint1, circle, RS_Vector(125.0, 10.0));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==1 );
        
        saveAndCompare(&ec, "modification_divide2033");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2033: OK");
    }
    
    /**
     * divide2: circle / normal case / around 0 degree area
     */
    void testDivide2034() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2034");
        
        RS_Modification modification(ec);
        
        RS_Circle* circle = new RS_Circle(&ec, 
            RS_CircleData(RS_Vector(125,125), 
                100.0));
        ec.addEntity(circle);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(355.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(5.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint1, cutPoint2, circle, 
            RS_Vector(125.0+105.0, 125.0));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==1 );
        
        saveAndCompare(&ec, "modification_divide2034");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2034: OK");
    }
    
    /**
     * divide2: circle / normal case 2 / around 0 degree area
     */
    void testDivide2035() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2035");
        
        RS_Modification modification(ec);
        
        RS_Circle* circle = new RS_Circle(&ec, 
            RS_CircleData(RS_Vector(125,125), 
                100.0));
        ec.addEntity(circle);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(355.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(5.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint2, cutPoint1, circle, 
            RS_Vector(125.0+105.0, 125.0));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==1 );
        
        saveAndCompare(&ec, "modification_divide2035");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2035: OK");
    }
    
    /**
     * divide2: circle / normal case / around 0 degree area / cut larger segment
     */
    void testDivide2036() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2036");
        
        RS_Modification modification(ec);
        
        RS_Circle* circle = new RS_Circle(&ec, 
            RS_CircleData(RS_Vector(125,125), 
                100.0));
        ec.addEntity(circle);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(355.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(5.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint1, cutPoint2, circle, 
            RS_Vector(125.0-105.0, 125.0));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==1 );
        
        saveAndCompare(&ec, "modification_divide2036");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2036: OK");
    }
    
    /**
     * divide2: circle / normal case 2 / around 0 degree area / cut larger segment
     */
    void testDivide2037() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2037");
        
        RS_Modification modification(ec);
        
        RS_Circle* circle = new RS_Circle(&ec, 
            RS_CircleData(RS_Vector(125,125), 
                100.0));
        ec.addEntity(circle);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        RS_Vector cutPoint2;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(355.0));
        cutPoint2.setPolar(100.0, RS_Math::deg2rad(5.0));
        cutPoint1.move(RS_Vector(125,125));
        cutPoint2.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint2, cutPoint1, circle, 
            RS_Vector(125.0-105.0, 125.0));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==1 );
        
        saveAndCompare(&ec, "modification_divide2037");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2037: OK");
    }
    
    /**
     * divide2: circle / only one cutting point
     */
    void testDivide2038() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2038");
        
        RS_Modification modification(ec);
        
        RS_Circle* circle = new RS_Circle(&ec, 
            RS_CircleData(RS_Vector(125,125), 
                100.0));
        ec.addEntity(circle);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        RS_Vector cutPoint1;
        cutPoint1.setPolar(100.0, RS_Math::deg2rad(85.0));
        cutPoint1.move(RS_Vector(125,125));
        
        modification.divide2(cutPoint1, RS_Vector(false), circle, RS_Vector(125.0, 230.0));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==0 );
        
        saveAndCompare(&ec, "modification_divide2038");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2038: OK");
    }
    
    /**
     * divide2: circle / no cutting point
     */
    void testDivide2039() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2039");
        
        RS_Modification modification(ec);
        
        RS_Circle* circle = new RS_Circle(&ec, 
            RS_CircleData(RS_Vector(125,125), 
                100.0));
        ec.addEntity(circle);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        modification.divide2(RS_Vector(false), RS_Vector(false), circle, RS_Vector(125.0, 230.0));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==0 );
        
        saveAndCompare(&ec, "modification_divide2039");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2039: OK");
    }
    
    /**
     * divide2: divide2 with limiting entities / line
     */
    void testDivide2050() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2050");
        
        RS_Modification modification(ec);
        
        RS_Line* line = new RS_Line(&ec, 
            RS_LineData(RS_Vector(10,10), RS_Vector(240,10)));
        ec.addEntity(line);
        
        RS_Line* lim1 = new RS_Line(&ec, 
            RS_LineData(RS_Vector(125-10,5), RS_Vector(125-10,15)));
        ec.addEntity(lim1);
        
        RS_Line* lim2 = new RS_Line(&ec, 
            RS_LineData(RS_Vector(125+10,5), RS_Vector(125+10,15)));
        ec.addEntity(lim2);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        modification.divide2(line, RS_Vector(125,15));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==4 );
        
        saveAndCompare(&ec, "modification_divide2050");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2050: OK");
    }
    
    /**
     * divide2: divide2 with limiting entities / line / many intersections
     */
    void testDivide2051() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2051");
        
        RS_Modification modification(ec);
        
        RS_Line* line = new RS_Line(&ec, 
            RS_LineData(RS_Vector(10,10), RS_Vector(240,10)));
        ec.addEntity(line);
        
        int x;
        RS_Line* lim1;
        RS_Line* lim2;

        for (x=20; x<125-10; x+=5) {
            lim1 = new RS_Line(&ec, 
                RS_LineData(RS_Vector(x,5), RS_Vector(x,15)));
            ec.addEntity(lim1);
        }
        
        for (x=125+10; x<230; x+=5) {
            lim2 = new RS_Line(&ec, 
                RS_LineData(RS_Vector(x,5), RS_Vector(x,15)));
            ec.addEntity(lim2);
        }

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        modification.divide2(line, RS_Vector(125,15));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        //CPPUNIT_ASSERT( ec.count()==4 );
        
        saveAndCompare(&ec, "modification_divide2051");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2051: OK");
    }
    
    /**
     * divide2: divide2 without limiting entities / line / delete whole line
     */
    void testDivide2052() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2052");
        
        RS_Modification modification(ec);
        
        RS_Line* line = new RS_Line(&ec, 
            RS_LineData(RS_Vector(10,10), RS_Vector(240,10)));
        ec.addEntity(line);
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        modification.divide2(line, RS_Vector(125,15));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==0 );
        
        saveAndCompare(&ec, "modification_divide2052");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2052: OK");
    }
    
    /**
     * divide2: divide2 with one limiting entity / line / delete segment until endpoint
     */
    void testDivide2053() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2053");
        
        RS_Modification modification(ec);
        
        RS_Line* line = new RS_Line(&ec, 
            RS_LineData(RS_Vector(10,10), RS_Vector(240,10)));
        ec.addEntity(line);
        
        RS_Line* lim1 = new RS_Line(&ec, 
            RS_LineData(RS_Vector(125-10,5), RS_Vector(125-10,15)));
        ec.addEntity(lim1);
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        modification.divide2(line, RS_Vector(125,15));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==2 );
        
        saveAndCompare(&ec, "modification_divide2053");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2053: OK");
    }
    
    /**
     * divide2: divide2 with limiting entities / arc
     */
    void testDivide2060() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2060");
        
        RS_Modification modification(ec);
        
        RS_Arc* arc = new RS_Arc(&ec, 
            RS_ArcData(RS_Vector(125,125), 
                100, 
                RS_Math::deg2rad(45.0),
                RS_Math::deg2rad(135.0),
                false));
        ec.addEntity(arc);
        
        RS_Line* lim1 = new RS_Line(&ec, 
            RS_LineData(RS_Vector(125+10,125+90), RS_Vector(125+10,125+110)));
        ec.addEntity(lim1);
        
        RS_Line* lim2 = new RS_Line(&ec, 
            RS_LineData(RS_Vector(125-10,125+90), RS_Vector(125-10,125+110)));
        ec.addEntity(lim2);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        modification.divide2(arc, RS_Vector(125,125+110));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==4 );
        
        saveAndCompare(&ec, "modification_divide2060");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2060: OK");
    }
    
    /**
     * divide2: divide2 with limiting entities / arc
     */
    void testDivide2061() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2061");
        
        RS_Modification modification(ec);
        
        RS_Arc* arc = new RS_Arc(&ec, 
            RS_ArcData(RS_Vector(125,125), 
                100, 
                RS_Math::deg2rad(360.0-45.0),
                RS_Math::deg2rad(45.0),
                false));
        ec.addEntity(arc);
        
        RS_Line* lim1 = new RS_Line(&ec, 
            RS_LineData(RS_Vector(125+90,125+10), RS_Vector(125+110,125+10)));
        ec.addEntity(lim1);
        
        RS_Line* lim2 = new RS_Line(&ec, 
            RS_LineData(RS_Vector(125+90,125-10), RS_Vector(125+110,125-10)));
        ec.addEntity(lim2);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        modification.divide2(arc, RS_Vector(125+110,125));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==4 );
        
        saveAndCompare(&ec, "modification_divide2061");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2061: OK");
    }
    
    /**
     * divide2: divide2 with limiting entities / circle
     */
    void testDivide2070() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2070");
        
        RS_Modification modification(ec);
        
        RS_Circle* circle = new RS_Circle(&ec, 
            RS_CircleData(RS_Vector(125,125), 
                100));
        ec.addEntity(circle);
        
        RS_Line* lim1 = new RS_Line(&ec, 
            RS_LineData(RS_Vector(125+10,125+90), RS_Vector(125+10,125+110)));
        ec.addEntity(lim1);
        
        RS_Line* lim2 = new RS_Line(&ec, 
            RS_LineData(RS_Vector(125-10,125+90), RS_Vector(125-10,125+110)));
        ec.addEntity(lim2);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        modification.divide2(circle, RS_Vector(125,125+110));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==3 );
        
        saveAndCompare(&ec, "modification_divide2070");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2070: OK");
    }
    
    /**
     * divide2: divide2 with limiting entities / circle
     */
    void testDivide2071() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2071");
        
        RS_Modification modification(ec);
        
        RS_Circle* circle = new RS_Circle(&ec, 
            RS_CircleData(RS_Vector(125,125), 
                100));
        ec.addEntity(circle);
        
        RS_Line* lim1 = new RS_Line(&ec, 
            RS_LineData(RS_Vector(125+90,125+10), RS_Vector(125+110,125+10)));
        ec.addEntity(lim1);
        
        RS_Line* lim2 = new RS_Line(&ec, 
            RS_LineData(RS_Vector(125+90,125-10), RS_Vector(125+110,125-10)));
        ec.addEntity(lim2);

        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);

        modification.divide2(circle, RS_Vector(125+110,125));
        
        RS_DEBUG->printEntity(RS_Debug::D_TESTING, &ec);
        
        CPPUNIT_ASSERT( ec.count()==3 );
        
        saveAndCompare(&ec, "modification_divide2071");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testDivide2071: OK");
    }
    

protected:
    RS_EntityContainer ec;
};
