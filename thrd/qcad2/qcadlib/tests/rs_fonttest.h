#include <stdio.h>
#include "rs_test.h"
#include "rs_font.h"

#include <QDateTime>

class RS_FontTest : public RS_Test  {
    CPPUNIT_TEST_SUITE( RS_FontTest );
    CPPUNIT_TEST( testStandard );
    //CPPUNIT_TEST( testUnicode );
    CPPUNIT_TEST( testFontList );
    CPPUNIT_TEST_SUITE_END();

private:

public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_FontTest");
        RS_FONTLIST->init();
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_FontTest: OK");
        RS_FONTLIST->clearFonts();
    }

    void testStandard() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testStandard");

        RS_Font* font = RS_FONTLIST->requestFont("Standard");
        CPPUNIT_ASSERT(font!=NULL);
        CPPUNIT_ASSERT(font->getFileName()=="standard");
        CPPUNIT_ASSERT(font->getLetterSpacing()==3.0);
        CPPUNIT_ASSERT(font->getWordSpacing()==6.75);
        CPPUNIT_ASSERT(font->getLineSpacingFactor()==1.0);
        CPPUNIT_ASSERT(font->countLetters()==105);

        RS_Block* blk;
        for (int i=0; i<font->countLetters(); ++i) {
            blk = font->letterAt(i);
            CPPUNIT_ASSERT(blk!=NULL);
            RS_DEBUG->print("RS_FontTest::test: letter with "
                "name '%s' [%d] found", 
                (const char*)blk->getName().toLatin1(),
                blk->getName()[0].unicode());
        }

        RS_Entity* e0;
        RS_Entity* e1;
        RS_Entity* e2;

        // check letter A:
        RS_Block* letterA = font->findLetter("A");
        CPPUNIT_ASSERT(letterA!=NULL);
        CPPUNIT_ASSERT(letterA->count()==3);
        e0 = letterA->entityAt(0);
        e1 = letterA->entityAt(1);
        e2 = letterA->entityAt(2);

        CPPUNIT_ASSERT(e0!=NULL);
        CPPUNIT_ASSERT(e1!=NULL);
        CPPUNIT_ASSERT(e2!=NULL);

        CPPUNIT_ASSERT(e0->rtti()==RS2::EntityLine);
        CPPUNIT_ASSERT(e1->rtti()==RS2::EntityLine);
        CPPUNIT_ASSERT(e2->rtti()==RS2::EntityLine);

        CPPUNIT_ASSERT( dynamic_cast<RS_Line*>(e0)->getStartpoint()
                        .distanceTo(RS_Vector(0,0)) < 1.0e-4 );
        CPPUNIT_ASSERT( dynamic_cast<RS_Line*>(e0)->getEndpoint()
                        .distanceTo(RS_Vector(3,9)) < 1.0e-4 );

        // test division sign:
        RS_Block* letterDiv = font->findLetter(RS_String(RS_Char(247)));
        CPPUNIT_ASSERT(letterDiv!=NULL);
        RS_DEBUG->print(RS_Debug::D_TESTING, "count: %d", letterDiv->count());
        CPPUNIT_ASSERT(letterDiv->count()==3);

        RS_DEBUG->print(RS_Debug::D_TESTING, "testStandard: OK");
    }
    
    
    void testUnicode() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testUnicode");

        RS_DEBUG->setLevel(RS_Debug::D_TESTING);
        QDateTime start = QDateTime::currentDateTime();

        RS_Font* font = RS_FONTLIST->requestFont("Unicode");
        CPPUNIT_ASSERT(font!=NULL);
        CPPUNIT_ASSERT(font->getFileName()=="unicode");
        //CPPUNIT_ASSERT(font->countLetters()==105);
        
        RS_Block* blk;
        for (int i=0; i<font->countLetters(); ++i) {
            blk = font->letterAt(i);
            CPPUNIT_ASSERT(blk!=NULL);
            RS_DEBUG->print(RS_Debug::D_TESTING, 
                "RS_FontTest::test: letter with "
                "unicode '%d' found", blk->getName().at(0).unicode());
        }
        
        RS_Char c1(20523);
        RS_Char c2(65533);
        for (int k=0; k<50000; k++) {
            blk = font->findLetter(RS_String(c1));
            blk = font->findLetter(RS_String(c2));
        }
        CPPUNIT_ASSERT(blk!=NULL);
        
        QDateTime stop = QDateTime::currentDateTime();
        RS_DEBUG->setLevel(RS_Debug::D_DEBUGGING);

        RS_DEBUG->print(RS_Debug::D_TESTING, "seconds: %d (37s)",
            start.secsTo(stop));
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testUnicode: OK");
    }

    void testFontList() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testFontList");
        for (RS_Font* f = RS_FONTLIST->firstFont();
                f!=NULL;
                f = RS_FONTLIST->nextFont()) {

            RS_DEBUG->print(RS_Debug::D_TESTING, 
                "font file: %s", 
                (const char*)f->getFileName().toLatin1());
        }
        RS_DEBUG->print(RS_Debug::D_TESTING, "testFontList: OK");
    }

};
