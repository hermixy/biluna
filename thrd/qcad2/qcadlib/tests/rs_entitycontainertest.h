#include <stdio.h>
#include <cppunit/extensions/HelperMacros.h>
#include "rs_entitycontainer.h"
#include "rs_line.h"

#define TESTTOL 1.0e-10

class RS_EntityContainerTest : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( RS_EntityContainerTest );
    CPPUNIT_TEST( testAddEntity );
    CPPUNIT_TEST( testFirstEntity );
    CPPUNIT_TEST( testIteration );
    CPPUNIT_TEST( testClone );
    CPPUNIT_TEST( testMoveEntityToFront );
    CPPUNIT_TEST( testMoveEntityToBack );
    CPPUNIT_TEST( testPrevEntity );
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_EntityContainerTest");
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_EntityContainerTest: OK");
    }
    
    void testFirstEntity() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testFirstEntity");
        RS_EntityContainer* ec;
        ec = new RS_EntityContainer();
        CPPUNIT_ASSERT(ec->firstEntity()==NULL);
        RS_Line* first = new RS_Line(ec, 
                    RS_LineData(RS_Vector(2,5), 
                                RS_Vector(12,15)));
        ec->addEntity(first);
        CPPUNIT_ASSERT(ec->firstEntity()==first);
        delete ec;
        RS_DEBUG->print(RS_Debug::D_TESTING, "testFirstEntity: OK");
    }

    void testAddEntity() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testAddEntity");
        RS_EntityContainer* ec;
        
        for (int k=0; k<50; ++k) {
            ec = new RS_EntityContainer();

            for (int i=1; i<10000; ++i) {
                ec->addEntity(
                    new RS_Line(ec, 
                            RS_LineData(RS_Vector(2,5), 
                                        RS_Vector(12,15))));
                            
                CPPUNIT_ASSERT( ec->count()==i );
            }

            delete ec;
        }

        RS_DEBUG->print(RS_Debug::D_TESTING, "testAddEntity: OK");
    }
    
    void testIteration() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testIteration");
        RS_EntityContainer* ec;
        
        ec = new RS_EntityContainer();

        for (int i=0; i<100; ++i) {
            ec->addEntity(
                new RS_Line(ec, 
                        RS_LineData(RS_Vector(2,5), 
                                    RS_Vector(12,15))));
                        
        }

        int c=0;
        for (RS_Entity* e=ec->firstEntity();
             e!=NULL;
             e=ec->nextEntity()) {
            CPPUNIT_ASSERT( e->rtti()==RS2::EntityLine );
            ++c;
        }
        CPPUNIT_ASSERT( c==100 );

        delete ec;

        RS_DEBUG->print(RS_Debug::D_TESTING, "testIteration: OK");
    }
    
    void testClone() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testClone");
        RS_EntityContainer* ec;
        ec = new RS_EntityContainer();
        for (int i=0; i<10; ++i) {
            ec->addEntity(
                new RS_Line(ec, 
                        RS_LineData(RS_Vector(2,5), 
                                    RS_Vector(12,15))));
        }
        CPPUNIT_ASSERT( ec->count()==10 );
        
        RS_EntityContainer* clone = dynamic_cast<RS_EntityContainer*>(ec->clone());
        CPPUNIT_ASSERT( clone->count()==10 );
        delete ec;
        
        CPPUNIT_ASSERT( clone->count()==10 );
        CPPUNIT_ASSERT( clone->firstEntity() != NULL );
        CPPUNIT_ASSERT( clone->firstEntity()->rtti()==RS2::EntityLine );
        CPPUNIT_ASSERT( clone->nextEntity()->rtti()==RS2::EntityLine );
        delete clone;
        RS_DEBUG->print(RS_Debug::D_TESTING, "testClone: OK");
    }
    
    void testMoveEntityToBack() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMoveEntityToBack");

        RS_EntityContainer* ec;
        ec = new RS_EntityContainer();
        for (int i=0; i<10; ++i) {
            ec->addEntity(
                new RS_Line(ec, 
                        RS_LineData(RS_Vector(0,i), 
                                    RS_Vector(10,i))));
        }
        CPPUNIT_ASSERT( ec->count()==10 );
        
        ec->moveEntityToBack(ec->entityAt(2));
        CPPUNIT_ASSERT( ec->firstEntity()->rtti()==RS2::EntityLine );
        CPPUNIT_ASSERT( dynamic_cast<RS_Line*>(ec->firstEntity())->getStartpoint().distanceTo(RS_Vector(0,2)) < 1.0e-10 );
        
        ec->moveEntityToBack(ec->entityAt(0));
        CPPUNIT_ASSERT( dynamic_cast<RS_Line*>(ec->firstEntity())->getStartpoint().distanceTo(RS_Vector(0,2)) < 1.0e-10 );
        
        ec->moveEntityToBack(ec->entityAt(9));
        CPPUNIT_ASSERT( dynamic_cast<RS_Line*>(ec->firstEntity())->getStartpoint().distanceTo(RS_Vector(0,9)) < 1.0e-10 );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMoveEntityToBack: OK");
    }
    
    void testMoveEntityToFront() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMoveEntityToFront");

        RS_EntityContainer* ec;
        ec = new RS_EntityContainer();
        for (int i=0; i<10; ++i) {
            ec->addEntity(
                new RS_Line(ec, 
                        RS_LineData(RS_Vector(0,i), 
                                    RS_Vector(10,i))));
        }
        CPPUNIT_ASSERT( ec->count()==10 );
        
        ec->moveEntityToFront(ec->entityAt(2));
        CPPUNIT_ASSERT( ec->firstEntity()->rtti()==RS2::EntityLine );
        CPPUNIT_ASSERT( dynamic_cast<RS_Line*>(ec->lastEntity())->getStartpoint().distanceTo(RS_Vector(0,2)) < 1.0e-10 );
        
        ec->moveEntityToFront(ec->entityAt(9));
        CPPUNIT_ASSERT( dynamic_cast<RS_Line*>(ec->lastEntity())->getStartpoint().distanceTo(RS_Vector(0,2)) < 1.0e-10 );
        
        ec->moveEntityToFront(ec->entityAt(0));
        CPPUNIT_ASSERT( dynamic_cast<RS_Line*>(ec->lastEntity())->getStartpoint().distanceTo(RS_Vector(0,0)) < 1.0e-10 );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testMoveEntityToFront: OK");
    }
    
    void testPrevEntity() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testPrevEntity");
        RS_EntityContainer* ec;
        ec = new RS_EntityContainer();
        CPPUNIT_ASSERT(ec->firstEntity()==NULL);
        RS_Line* first = new RS_Line(ec, 
                    RS_LineData(RS_Vector(0,1), 
                                RS_Vector(10,1)));
        RS_Line* second = new RS_Line(ec, 
                    RS_LineData(RS_Vector(0,2), 
                                RS_Vector(10,2)));
        RS_Line* third = new RS_Line(ec, 
                    RS_LineData(RS_Vector(0,3), 
                                RS_Vector(10,3)));
        ec->addEntity(first);
        ec->addEntity(second);
        ec->addEntity(third);
        
        ec->findEntity(second);
        CPPUNIT_ASSERT(ec->currentEntity()==second);
        CPPUNIT_ASSERT(ec->prevEntity()==first);
        

        CPPUNIT_ASSERT(ec->firstEntity()==first);
        CPPUNIT_ASSERT(ec->lastEntity()==third);
        delete ec;
        RS_DEBUG->print(RS_Debug::D_TESTING, "testPrevEntity: OK");
    }
};
