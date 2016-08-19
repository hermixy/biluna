//#include "rs_testqcadprof.h"
#include "rs.h"
#include "rs_test.h"
#include "rs_creation.h"
//#include "rs_fileinfo.h"

class RS_CreationEquidistantTest : public RS_Test {
    CPPUNIT_TEST_SUITE( RS_CreationEquidistantTest );
    CPPUNIT_TEST( testCreateEquidistant );
    CPPUNIT_TEST_SUITE_END();

private:
    RS_EntityContainer* ec;
    RS_Creation* creation;
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_CreationEquidistantTest");
        ec = new RS_EntityContainer();
        creation = new RS_Creation(ec);
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_CreationEquidistantTest: OK");
        delete ec;
        delete creation;
        ec = NULL;
        creation = NULL;
    }
    
    void testCreateEquidistant() {
        for (int i=0; i<=1000; ++i) {
            if (RS_FileInfo(RS_String("data/input/equidistant%1.dxf")
                .arg(i, 3, 10, RS_Char('0'))).exists()) {
                testCreateEquidistant(i, true);
                testCreateEquidistant(i, false);
            }
            else {
                break;
            }
        }
    }
    
    /**
     * Loads data/polyline<num>.dxf and creates equidistant polylines at all
     * points found in the file.
     */
    void testCreateEquidistant(int num, bool useArcs) {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testCreateEquidistant %03d", num);

        RS_String name = RS_String("equidistant%1").arg(num, 3, 10, RS_Char('0'));
        RS_String outputName;
        if (useArcs) {
            outputName = name + "_arcs";
        }
        else {
            outputName = name;
        }
        RS_String inputFile = RS_String("data/input/%1.dxf").arg(name);
        
        RS_Graphic g;
        RS_Creation cr(&g);
        
        g.open(inputFile, RS2::FormatDxf);

        // find polyline:
        RS_Polyline* pl = NULL;
        for (RS_Entity* e=g.firstEntity(); e!=NULL; e=g.nextEntity()) {
            if (e->rtti()==RS2::EntityPolyline) {
                pl = dynamic_cast<RS_Polyline*>(e);
                break;
            }
        }

        CPPUNIT_ASSERT(pl!=NULL);

        // create equidistant for every point:
        double d = 10.0;
        RS_DEBUG->print(RS_Debug::D_TESTING, 
            "testCreateEquidistant %03d: distance: %f", num, d);
        for (RS_Entity* e=g.firstEntity(); e!=NULL; e=g.nextEntity()) {
            RS_DEBUG->print(RS_Debug::D_TESTING, "looking for point");
            if (e->rtti()==RS2::EntityPoint) {
                RS_DEBUG->print(RS_Debug::D_TESTING, "createEquidistant");
                cr.createEquidistant(dynamic_cast<RS_Point*>(e)->getPos(), d, 1, pl, useArcs);
                break;
            }
        }
            
        //checkForZeroEntities(&g);
        saveAndCompare(&g, outputName, true);
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testCreateEquidistant %03d: OK", num);
    }
};
