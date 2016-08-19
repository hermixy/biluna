#include <stdio.h>
#include "rs_test.h"
#include "rs_text.h"

class RS_TextTest : public RS_Test  {
    CPPUNIT_TEST_SUITE( RS_TextTest );
    CPPUNIT_TEST( testRendering000 );
    CPPUNIT_TEST( testRendering001 );
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_TextTest");
        RS_FONTLIST->init();
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_TextTest: OK");
    }
    
    void testRendering000() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering000");
        
        RS_EntityContainer ec;
        
        RS_Text* text = new RS_Text(&ec, 
            RS_TextData(RS_Vector(10,125),
                        30.0,
                        100.0,
                        RS2::VAlignTop,
                        RS2::HAlignLeft,
                        RS2::LeftToRight,
                        RS2::Exact,
                        1.0,
                        "Test",
                        "Standard",
                        0.0));
        text->setPen(
            RS_Pen(RS_Color(255,0,0), RS2::Width01, RS2::SolidLine));
        ec.addEntity(text);
        
        saveAndCompare(&ec, "text000");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering000: OK");
    }
    
    
    void testRendering001() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering001");
        
        RS_EntityContainer ec;

        RS_String s;
        s = RS_String("ABC%1%2XYZ").arg(RS_Char(0x00d7)).arg(RS_Char(0x00f7));
        
        RS_Text* text = new RS_Text(&ec, 
            RS_TextData(RS_Vector(10,125),
                        30.0,
                        100.0,
                        RS2::VAlignTop,
                        RS2::HAlignLeft,
                        RS2::LeftToRight,
                        RS2::Exact,
                        1.0,
                        s,
                        "Standard",
                        0.0));
        text->setPen(
            RS_Pen(RS_Color(255,0,0), RS2::Width01, RS2::SolidLine));
        ec.addEntity(text);

        saveAndCompare(&ec, "text001");
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRendering001: OK");
    }
    
};
