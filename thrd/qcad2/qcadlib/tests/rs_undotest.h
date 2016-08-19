#include <stdio.h>
#include <cppunit/extensions/HelperMacros.h>
#include "rs_undo.h"
#include "rs_undoable.h"

/**
 * Test item (could be a drawing entity for instance).
 */
class RS_UndoableTestItem : public RS_Undoable {
public:
    RS_UndoableTestItem() {
        RS_DEBUG->print(RS_Debug::D_TESTING, 
            "RS_UndoableTestItem");
        instanceCounter++;
    }

    ~RS_UndoableTestItem() {
        RS_DEBUG->print(RS_Debug::D_TESTING, 
            "~RS_UndoableTestItem");
        instanceCounter--;
    }
    
    virtual RS2::UndoableType undoRtti() const {
        return (RS2::UndoableType)123;
    }
    
	virtual void undoStateChanged(bool undone) {
        RS_DEBUG->print(RS_Debug::D_TESTING, 
            "RS_UndoableTestItem: undone: %d", (int)undone);
    }

    static void resetInstanceCounter() {
        instanceCounter = 0;
    }

    static int getInstanceCounter() {
        return instanceCounter;
    }

private:
    static int instanceCounter;
};

int RS_UndoableTestItem::instanceCounter = 0;

/**
 * Test container (could be an entity container for instance).
 */
class RS_UndoTestContainer : public RS_Undo {
public:
    RS_UndoTestContainer() {
        undoables = new RS_PtrList<RS_UndoableTestItem>();
        undoables->setAutoDelete(true);
    }

    virtual void addUndoable(RS_Undoable* u) {
        CPPUNIT_ASSERT( u!=NULL );
        CPPUNIT_ASSERT( u->undoRtti()==123 );
        undoables->append((RS_UndoableTestItem*)u);
        RS_Undo::addUndoable(u);
    }

    virtual void removeUndoable(RS_Undoable* u) {
        CPPUNIT_ASSERT( u!=NULL );
        CPPUNIT_ASSERT( u->undoRtti()==123 );
        undoables->remove((RS_UndoableTestItem*)u);
    }

private:
    RS_PtrList<RS_UndoableTestItem>* undoables;
};


/**
 * Test case.
 */
class RS_UndoTest : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( RS_UndoTest );
    CPPUNIT_TEST( testInstanceCounter );
    CPPUNIT_TEST( testUndo );
    CPPUNIT_TEST( testRedo );
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_UndoTest");
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_UndoTest: OK");
    }
    
    void testInstanceCounter() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testInstanceCounter");

        RS_UndoableTestItem::resetInstanceCounter();
        CPPUNIT_ASSERT( RS_UndoableTestItem::getInstanceCounter()==0 );
        RS_Undoable* u = new RS_UndoableTestItem();
        CPPUNIT_ASSERT( RS_UndoableTestItem::getInstanceCounter()==1 );
        delete u;
        CPPUNIT_ASSERT( RS_UndoableTestItem::getInstanceCounter()==0 );

        RS_DEBUG->print(RS_Debug::D_TESTING, "testInstanceCounter: OK");
    }
    
    void testUndo() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testUndo");

        RS_UndoableTestItem::resetInstanceCounter();
        CPPUNIT_ASSERT( RS_UndoableTestItem::getInstanceCounter()==0 );

        RS_UndoTestContainer utc;
        CPPUNIT_ASSERT( utc.countUndoCycles()==0 );
        CPPUNIT_ASSERT( utc.countRedoCycles()==0 );
        
        utc.startUndoCycle();
        RS_Undoable* u0 = new RS_UndoableTestItem();
        utc.addUndoable(u0);
        utc.endUndoCycle();
        
        CPPUNIT_ASSERT( RS_UndoableTestItem::getInstanceCounter()==1 );
        CPPUNIT_ASSERT( utc.countUndoCycles()==1 );
        CPPUNIT_ASSERT( utc.countRedoCycles()==0 );
        
        utc.startUndoCycle();
        RS_Undoable* u1 = new RS_UndoableTestItem();
        utc.addUndoable(u1);
        utc.endUndoCycle();

        CPPUNIT_ASSERT( RS_UndoableTestItem::getInstanceCounter()==2 );
        CPPUNIT_ASSERT( utc.countUndoCycles()==2 );
        CPPUNIT_ASSERT( utc.countRedoCycles()==0 );
        
        utc.undo();

        CPPUNIT_ASSERT( RS_UndoableTestItem::getInstanceCounter()==2 );
        CPPUNIT_ASSERT( utc.countUndoCycles()==1 );
        CPPUNIT_ASSERT( utc.countRedoCycles()==1 );
        
        utc.startUndoCycle();
        RS_Undoable* u2 = new RS_UndoableTestItem();
        utc.addUndoable(u2);
        utc.endUndoCycle();
        
        CPPUNIT_ASSERT( RS_UndoableTestItem::getInstanceCounter()==2 );
        CPPUNIT_ASSERT( utc.countUndoCycles()==2 );
        CPPUNIT_ASSERT( utc.countRedoCycles()==0 );

        RS_DEBUG->print(RS_Debug::D_TESTING, "testUndo: OK");
    }
    
    void testRedo() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRedo");

        RS_UndoableTestItem::resetInstanceCounter();
        CPPUNIT_ASSERT( RS_UndoableTestItem::getInstanceCounter()==0 );

        RS_UndoTestContainer utc;
        CPPUNIT_ASSERT( utc.countUndoCycles()==0 );
        CPPUNIT_ASSERT( utc.countRedoCycles()==0 );
        
        utc.startUndoCycle();
        RS_Undoable* u0 = new RS_UndoableTestItem();
        utc.addUndoable(u0);
        utc.endUndoCycle();
        
        CPPUNIT_ASSERT( RS_UndoableTestItem::getInstanceCounter()==1 );
        CPPUNIT_ASSERT( utc.countUndoCycles()==1 );
        CPPUNIT_ASSERT( utc.countRedoCycles()==0 );
        
        utc.startUndoCycle();
        RS_Undoable* u1 = new RS_UndoableTestItem();
        utc.addUndoable(u1);
        utc.endUndoCycle();

        CPPUNIT_ASSERT( RS_UndoableTestItem::getInstanceCounter()==2 );
        CPPUNIT_ASSERT( utc.countUndoCycles()==2 );
        CPPUNIT_ASSERT( utc.countRedoCycles()==0 );
        
        utc.undo();

        CPPUNIT_ASSERT( RS_UndoableTestItem::getInstanceCounter()==2 );
        CPPUNIT_ASSERT( utc.countUndoCycles()==1 );
        CPPUNIT_ASSERT( utc.countRedoCycles()==1 );
        
        utc.redo();
        
        CPPUNIT_ASSERT( RS_UndoableTestItem::getInstanceCounter()==2 );
        CPPUNIT_ASSERT( utc.countUndoCycles()==2 );
        CPPUNIT_ASSERT( utc.countRedoCycles()==0 );
        
        utc.startUndoCycle();
        RS_Undoable* u2 = new RS_UndoableTestItem();
        utc.addUndoable(u2);
        utc.endUndoCycle();
        
        CPPUNIT_ASSERT( RS_UndoableTestItem::getInstanceCounter()==3 );
        CPPUNIT_ASSERT( utc.countUndoCycles()==3 );
        CPPUNIT_ASSERT( utc.countRedoCycles()==0 );

        RS_DEBUG->print(RS_Debug::D_TESTING, "testRedo: OK");
    }
};
