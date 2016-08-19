#include <stdio.h>
#include "rs_test.h"
#include "rs_dimaligned.h"


class RS_DimensionTest : public RS_Test  {
    CPPUNIT_TEST_SUITE( RS_DimensionTest );
    CPPUNIT_TEST( testRenderingDim);
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_DimensionTest");
        RS_FONTLIST->init();
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_DimensionTest: OK");
        RS_FONTLIST->clearFonts();
    }
    
    void testRenderingDim() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRenderingDim");
        
        /*
        RS_EntityContainer ec;
        RS_DimAligned* d;

        d = new RS_DimAligned(&ec, 
            RS_DimensionData(
                RS_Vector(50,150),
                RS_Vector(false),
                RS2::VAlignBottom,
                RS2::HAlignCenter,
                RS2::Exact,
                1.0,
                "<>",
                "Standard",
                0.0
            ), 
            RS_DimAlignedData(
                RS_Vector(50,50),
                RS_Vector(200,50)
            ));
        d->overrideAttributeSize(10);
        d->update();
        ec.addEntity(d);
        saveAndCompare(&ec, "dimaligned000");
        */

        /*
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
        */

        RS_StringList types;
        /*
        types << "dimaligned";
        types << "dimhorizontal";
        types << "dimvertical";
        types << "dimlinear";
        types << "dimdiametric";
        types << "dimradial";
        types << "dimangular";
        */
        types << "dimordinate";
        
        for (int c=0; c<types.count(); ++c) {
            for (int i=0; i<=1000; ++i) {
                if (RS_FileInfo(RS_String("data/input/%1%2.dxf")
                    .arg(types.at(c)).arg(i, 3, 10, RS_Char('0'))).exists()) {
                    testRenderingDim(i, types.at(c));
                }
                else {
                    break;
                }
            }
        }
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering: OK");
    }
    
    
    void testRenderingDim(int num, const RS_String& type) {
        RS_DEBUG->print(RS_Debug::D_TESTING, 
            "testRenderingDim %s%03d", 
            (const char*)type.toLatin1(), num);

        RS_String name = RS_String("%1%2").arg(type).arg(num, 3, 10, RS_Char('0'));
        RS_String outputName = name;
        RS_String inputFile = RS_String("data/input/%1.dxf").arg(name);
        
        RS_Graphic g;
        g.open(inputFile, RS2::FormatDxf);
        saveAndCompare(&g, outputName, true);
        
        RS_DEBUG->print(RS_Debug::D_TESTING, 
            "testRenderingDim %s%03d: OK", 
            (const char*)type.toLatin1(), num);
    }
};
