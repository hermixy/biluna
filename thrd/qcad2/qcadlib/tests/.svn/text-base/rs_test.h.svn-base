#ifndef RS_TEST1
#define RS_TEST1

#include <cppunit/extensions/HelperMacros.h>
#include "rs_debug.h"
#include "rs_img.h"
#include "rs_graphic.h"
#include "rs_bitmapgraphicview.h"
#include "rs_line.h"
#include "rs_arc.h"
#include "rs_circle.h"

/**
 * Base class for some tests.
 */
class RS_Test : public CppUnit::TestFixture {
public:
    /**
     * Saves the given container 'ec' as './data/output/<name>.png' and compares
     * it to './data/expected/<name>.png'.
     */
    void saveAndCompare(RS_EntityContainer* ec, const RS_String& name, bool autoZoom=false) {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_Test::saveAndCompare: %s", 
            (const char*)name.toLatin1());

        RS_BitmapGraphicView gv(250,250);
        RS_String resultFile = RS_String("data/output/%1.png").arg(name);
        RS_String resultDxfFile = RS_String("data/output/%1.dxf").arg(name);
        RS_String expectedFile = RS_String("data/expected/%1.png").arg(name);
        if (!gv.save(resultFile, ec, autoZoom)) {
            CPPUNIT_FAIL((const char*)RS_String("RS_Test::saveAndCompare: cannot create '%1'")
                .arg(resultFile).toLatin1());
        }

        RS_Graphic g;
        for (RS_Entity* e=ec->firstEntity(); e!=NULL; e=ec->nextEntity()) {
            g.addEntity(e->clone());
        }
        g.saveAs(resultDxfFile, RS2::FormatDxf);
        
        RS_Img result(resultFile);
        RS_Img expected(expectedFile);

        RS_String msg = 
            RS_String("result does not match expected in file '%1'").arg(name);

        CPPUNIT_ASSERT_MESSAGE( (const char*)msg.toLatin1(), result==expected );
    }

    /**
     * Asserts that there are no zero length entities in a container.
     */
    void checkForZeroEntities(RS_EntityContainer* ec) {
        
        RS_DEBUG->print(RS_Debug::D_TESTING, 
            "RS_Test::checkForZeroEntities");
        
        for (RS_Entity* e=ec->firstEntity(RS2::ResolveAll); 
             e!=NULL; 
             e=ec->nextEntity(RS2::ResolveAll)) {
                
            RS_DEBUG->printEntity(RS_Debug::D_TESTING, e);

            switch (e->rtti()) {
            case RS2::EntityLine: {
                RS_Line* l = dynamic_cast<RS_Line*>(e);
                /*RS_DEBUG->print(RS_Debug::D_TESTING, 
                    "RS_Test::checkForZeroEntities: line "
                    "has length: %f. position: %f/%f to %f/%f", 
                    l->getLength(),
                    l->getStartpoint().x, 
                    l->getStartpoint().y, 
                    l->getEndpoint().x, 
                    l->getEndpoint().y);*/
                CPPUNIT_ASSERT(l->getLength()>RS_TOLERANCE);
                } break;
            
            case RS2::EntityArc: {
                RS_Arc* a = dynamic_cast<RS_Arc*>(e);
                /*RS_DEBUG->print(RS_Debug::D_TESTING, 
                    "RS_Test::checkForZeroEntities: arc "
                    "has radius: %f. angle span: %f", 
                    a->getRadius(), a->getAngleLength()); */
                CPPUNIT_ASSERT(a->getRadius()>1.0e-4);
                CPPUNIT_ASSERT(a->getAngleLength()>RS_TOLERANCE_ANGLE);
                } break;
                
            case RS2::EntityCircle: {
                RS_Circle* c = dynamic_cast<RS_Circle*>(e);
                /*RS_DEBUG->print(RS_Debug::D_TESTING, 
                    "RS_Test::checkForZeroEntities: circle "
                    "has radius: %f.", 
                    c->getRadius()); */
                CPPUNIT_ASSERT(c->getRadius()>RS_TOLERANCE);
                } break;
                
            default:
                break;
            }
        }
    }
    
};

#endif
