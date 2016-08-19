#include <stdio.h>
#include <cppunit/extensions/HelperMacros.h>
#include "rs_extptrlist.h"

/**
 * Test list item that remembers if it's destructor was called.
 */
class RS_ExtPtrListTestItem {
public:
    RS_ExtPtrListTestItem(int v) {
        this->v = v;
    }
    
    ~RS_ExtPtrListTestItem() {
        dc = v;
    }

    static void resetItemDestructorCalled() {
        dc = -1;
    }
    static int itemDestructorCalled() {
        return dc;
    }
    int value() {
        return v;
    }

private:
    static int dc;
    int v;
};

int RS_ExtPtrListTestItem::dc = -1;

class RS_ExtPtrListTest : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( RS_ExtPtrListTest );
    CPPUNIT_TEST( testActiveItem );
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_ExtPtrListTest");
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_ExtPtrListTest: OK");
    }
    
    void testActiveItem() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testActiveItem");
        
        RS_ExtPtrList<int> l;
        l.setAutoDelete(true);

        CPPUNIT_ASSERT( l.getActive()==NULL );

        int* a;
        int* b;
        int* c;
        l.append(a = new int(3));
        l.append(b = new int(5));
        l.append(c = new int(7));
        
        CPPUNIT_ASSERT( l.getActive()==NULL );

        l.activate(a);
        CPPUNIT_ASSERT( l.getActive()==a );
        l.remove(a);
        CPPUNIT_ASSERT( l.getActive()==NULL );


        RS_DEBUG->print(RS_Debug::D_TESTING, "testActiveItem: OK");
    }
};


