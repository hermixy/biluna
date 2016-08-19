#include <stdio.h>
#include <cppunit/extensions/HelperMacros.h>
#include "rs_ptrlist.h"


/**
 * Test list item that remembers if it's destructor was called.
 */
class RS_PtrListTestItem {
public:
    RS_PtrListTestItem(int v) {
        this->v = v;
    }
    
    ~RS_PtrListTestItem() {
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

int RS_PtrListTestItem::dc = -1;


class RS_PtrListTest : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( RS_PtrListTest );
    CPPUNIT_TEST( testIteration );
    CPPUNIT_TEST( testNoAutoDelete );
    CPPUNIT_TEST( testAutoDelete );
    CPPUNIT_TEST( testCopy );
    CPPUNIT_TEST( testErase );
    CPPUNIT_TEST( testRemoveAt );
    CPPUNIT_TEST( testRemoveLast );
    CPPUNIT_TEST( testRemoveFirst );
    CPPUNIT_TEST( testReplace );
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_PtrListTest");
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_PtrListTest: OK");
    }
    
    void testIteration() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testIteration");
        
        RS_PtrList<int> l;
        l.setAutoDelete(true);
        CPPUNIT_ASSERT( l.first()==NULL );
        CPPUNIT_ASSERT( l.last()==NULL );
        CPPUNIT_ASSERT( l.next()==NULL );
        CPPUNIT_ASSERT( l.prev()==NULL );

        int* a = new int(3);
        int* b = new int(5);
        int* c = new int(7);
        
        l.append(a);
        l.append(b);
        l.append(c);
        
        CPPUNIT_ASSERT( *l.first()==3 );
        CPPUNIT_ASSERT( *l.next()==5 );
        
        CPPUNIT_ASSERT( *l.last()==7 );
        CPPUNIT_ASSERT( *l.prev()==5 );
        
        CPPUNIT_ASSERT( *l.current()==5 );
        CPPUNIT_ASSERT( l.currentIndex()==1 );

        l.first();
        CPPUNIT_ASSERT( l.currentIndex()==0 );
        CPPUNIT_ASSERT( *l.current()==3 );
        l.next();
        CPPUNIT_ASSERT( l.currentIndex()==1 );
        CPPUNIT_ASSERT( *l.current()==5 );
        l.next();
        CPPUNIT_ASSERT( l.currentIndex()==2 );
        CPPUNIT_ASSERT( *l.current()==7 );
        l.next();
        CPPUNIT_ASSERT( l.currentIndex()==-1 );
        CPPUNIT_ASSERT( l.current()==NULL );

        l.setCurrent(a);
        CPPUNIT_ASSERT( l.currentIndex()==0 );
        CPPUNIT_ASSERT( l.current()==a );
        l.setCurrent(b);
        CPPUNIT_ASSERT( l.currentIndex()==1 );
        CPPUNIT_ASSERT( l.current()==b );
        l.setCurrent(c);
        CPPUNIT_ASSERT( l.currentIndex()==2 );
        CPPUNIT_ASSERT( l.current()==c );
        
        l.setCurrent(b);
        CPPUNIT_ASSERT( l.current()==b );
        CPPUNIT_ASSERT( l.currentIndex()==1 );
        CPPUNIT_ASSERT( l.prev()==a );
        
        l.setCurrent(b);
        CPPUNIT_ASSERT( l.current()==b );
        CPPUNIT_ASSERT( l.currentIndex()==1 );
        CPPUNIT_ASSERT( l.next()==c );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testIteration: OK");
    }
    
    void testNoAutoDelete() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testNoAutoDelete");

        RS_PtrList<RS_PtrListTestItem> l;
        RS_PtrListTestItem* item3 = new RS_PtrListTestItem(3);
        RS_PtrListTestItem* item5 = new RS_PtrListTestItem(5);
        RS_PtrListTestItem* item7 = new RS_PtrListTestItem(7);
        l.append(item3);
        l.append(item5);
        l.append(item7);
        
        CPPUNIT_ASSERT( l.count()==3 );
        
        RS_PtrList<RS_PtrListTestItem>::iterator iter;
        iter = l.begin();
        RS_PtrListTestItem::resetItemDestructorCalled();
        l.erase(iter);
        CPPUNIT_ASSERT( RS_PtrListTestItem::itemDestructorCalled()==-1 );
        CPPUNIT_ASSERT( l.count()==2 );
        CPPUNIT_ASSERT( l.first()->value()==5 );
        
        RS_PtrListTestItem::resetItemDestructorCalled();
        l.clear();
        CPPUNIT_ASSERT( RS_PtrListTestItem::itemDestructorCalled()==-1 );

        CPPUNIT_ASSERT( l.count()==0 );

        delete item3;
        delete item5;
        delete item7;
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testNoAutoDelete: OK");
    }

    void testAutoDelete() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testAutoDelete");
        
        RS_PtrList<int> l;
        l.setAutoDelete(true);
        l.append(new int(3));
        l.append(new int(5));
        l.append(new int(7));

        CPPUNIT_ASSERT( l.count()==3 );
        l.clear();
        CPPUNIT_ASSERT( l.count()==0 );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testAutoDelete: OK");
    }
    
    void testCopy() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testCopy");

        RS_PtrList<int> l;
        l.setAutoDelete(true);
        l.append(new int(3));
        l.append(new int(5));
        l.append(new int(7));
        
        RS_PtrList<int> l2(l);
        l2.setAutoDelete(false);
        CPPUNIT_ASSERT( l.count()==3 );
        CPPUNIT_ASSERT( l2.count()==3 );
        
        CPPUNIT_ASSERT( *l.first()==3 );
        CPPUNIT_ASSERT( *l2.first()==3 );

        // change both original and copy:
        *l2.first() = -3;
        CPPUNIT_ASSERT( *l.first()==-3 );
        CPPUNIT_ASSERT( *l2.first()==-3 );

        l2.clear();
        CPPUNIT_ASSERT( l.count()==3 );
        CPPUNIT_ASSERT( l2.count()==0 );
        CPPUNIT_ASSERT( *l.first()==-3 );
        CPPUNIT_ASSERT( l2.first()==NULL );
        RS_DEBUG->print(RS_Debug::D_TESTING, "testCopy: OK");
    }
    
    void testErase() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testErase");

        RS_PtrList<RS_PtrListTestItem> l;
        l.setAutoDelete(true);
        RS_PtrList<RS_PtrListTestItem>::iterator iter;

        l.append(new RS_PtrListTestItem(3));
        l.append(new RS_PtrListTestItem(5));
        l.append(new RS_PtrListTestItem(7));
        
        CPPUNIT_ASSERT( l.count()==3 );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testErase: 001");

        iter = l.begin();
        RS_PtrListTestItem::resetItemDestructorCalled();
        l.erase(iter);
        CPPUNIT_ASSERT( RS_PtrListTestItem::itemDestructorCalled()==3 );
        CPPUNIT_ASSERT( l.count()==2 );
        CPPUNIT_ASSERT( l.first()->value()==5 );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testErase: 002");
        
        iter = l.end();
        iter--;
        RS_PtrListTestItem::resetItemDestructorCalled();
        l.erase(iter);
        CPPUNIT_ASSERT( RS_PtrListTestItem::itemDestructorCalled()==7 );
        CPPUNIT_ASSERT( l.count()==1 );
        CPPUNIT_ASSERT( l.first()->value()==5 );
        
        iter = l.end();
        RS_PtrListTestItem::resetItemDestructorCalled();
        l.erase(iter);
        CPPUNIT_ASSERT( RS_PtrListTestItem::itemDestructorCalled()==-1 );
        CPPUNIT_ASSERT( l.count()==1 );
        CPPUNIT_ASSERT( l.first()->value()==5 );

        RS_DEBUG->print(RS_Debug::D_TESTING, "testErase: OK");
    }
    
    void testRemoveAt() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRemoveAt");

        RS_PtrList<RS_PtrListTestItem> l;
        l.setAutoDelete(true);

        l.append(new RS_PtrListTestItem(3));
        l.append(new RS_PtrListTestItem(5));
        l.append(new RS_PtrListTestItem(7));
        
        CPPUNIT_ASSERT( l.count()==3 );

        RS_PtrListTestItem::resetItemDestructorCalled();
        l.removeAt(1);
        CPPUNIT_ASSERT( RS_PtrListTestItem::itemDestructorCalled()==5 );
        CPPUNIT_ASSERT( l.count()==2 );
        CPPUNIT_ASSERT( l.first()->value()==3 );
        CPPUNIT_ASSERT( l.last()->value()==7 );
        
        RS_PtrListTestItem::resetItemDestructorCalled();
        l.removeAt(0);
        CPPUNIT_ASSERT( RS_PtrListTestItem::itemDestructorCalled()==3 );
        CPPUNIT_ASSERT( l.count()==1 );
        CPPUNIT_ASSERT( l.first()->value()==7 );

        RS_DEBUG->print(RS_Debug::D_TESTING, "testRemoveAt: OK");
    }
        
    void testRemoveLast() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRemoveLast");
        
        RS_PtrList<RS_PtrListTestItem> l;
        l.setAutoDelete(true);
        l.append(new RS_PtrListTestItem(3));
        l.append(new RS_PtrListTestItem(5));
        l.append(new RS_PtrListTestItem(7));
        
        CPPUNIT_ASSERT( l.count()==3 );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRemoveLast: 001");
        
        RS_PtrListTestItem::resetItemDestructorCalled();
        l.removeLast();
        CPPUNIT_ASSERT( RS_PtrListTestItem::itemDestructorCalled()==7 );
        CPPUNIT_ASSERT( l.count()==2 );
        CPPUNIT_ASSERT( l.first()!=NULL );
        CPPUNIT_ASSERT( l.first()->value()==3 );
        CPPUNIT_ASSERT( l.last()->value()==5 );

        RS_DEBUG->print(RS_Debug::D_TESTING, "testRemoveLast: 002");
        
        RS_PtrListTestItem::resetItemDestructorCalled();
        l.removeLast();
        CPPUNIT_ASSERT( RS_PtrListTestItem::itemDestructorCalled()==5 );
        CPPUNIT_ASSERT( l.count()==1 );
        CPPUNIT_ASSERT( l.first()->value()==3 );
        CPPUNIT_ASSERT( l.last()->value()==3 );

        RS_DEBUG->print(RS_Debug::D_TESTING, "testRemoveLast: 003");
        
        RS_PtrListTestItem::resetItemDestructorCalled();
        l.removeLast();
        CPPUNIT_ASSERT( RS_PtrListTestItem::itemDestructorCalled()==3 );
        CPPUNIT_ASSERT( l.count()==0 );
        CPPUNIT_ASSERT( l.first()==NULL );
        CPPUNIT_ASSERT( l.last()==NULL );

        RS_DEBUG->print(RS_Debug::D_TESTING, "testRemoveLast: 004");
        
        RS_PtrListTestItem::resetItemDestructorCalled();
        l.removeLast();
        CPPUNIT_ASSERT( RS_PtrListTestItem::itemDestructorCalled()==-1 );
        CPPUNIT_ASSERT( l.count()==0 );
        CPPUNIT_ASSERT( l.last()==NULL );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRemoveLast: OK");
    }
    
    void testRemoveFirst() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRemoveFirst");
        
        RS_PtrList<RS_PtrListTestItem> l;
        l.setAutoDelete(true);
        l.append(new RS_PtrListTestItem(3));
        l.append(new RS_PtrListTestItem(5));
        l.append(new RS_PtrListTestItem(7));
        
        CPPUNIT_ASSERT( l.count()==3 );
        
        RS_PtrListTestItem::resetItemDestructorCalled();
        l.removeFirst();
        CPPUNIT_ASSERT( RS_PtrListTestItem::itemDestructorCalled()==3 );
        CPPUNIT_ASSERT( l.count()==2 );
        CPPUNIT_ASSERT( l.first()->value()==5 );
        CPPUNIT_ASSERT( l.last()->value()==7 );
        
        RS_PtrListTestItem::resetItemDestructorCalled();
        l.removeFirst();
        CPPUNIT_ASSERT( RS_PtrListTestItem::itemDestructorCalled()==5 );
        CPPUNIT_ASSERT( l.count()==1 );
        CPPUNIT_ASSERT( l.first()->value()==7 );
        CPPUNIT_ASSERT( l.last()->value()==7 );
        
        RS_PtrListTestItem::resetItemDestructorCalled();
        l.removeFirst();
        CPPUNIT_ASSERT( RS_PtrListTestItem::itemDestructorCalled()==7 );
        CPPUNIT_ASSERT( l.count()==0 );
        CPPUNIT_ASSERT( l.first()==NULL );
        CPPUNIT_ASSERT( l.last()==NULL );
        
        RS_PtrListTestItem::resetItemDestructorCalled();
        l.removeFirst();
        CPPUNIT_ASSERT( RS_PtrListTestItem::itemDestructorCalled()==-1 );
        CPPUNIT_ASSERT( l.count()==0 );
        CPPUNIT_ASSERT( l.first()==NULL );
        CPPUNIT_ASSERT( l.last()==NULL );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testRemoveFirst: OK");
    }
    
    void testReplace() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "testReplace");
        
        RS_PtrList<RS_PtrListTestItem> l;
        l.setAutoDelete(true);
        l.append(new RS_PtrListTestItem(3));
        l.append(new RS_PtrListTestItem(5));
        l.append(new RS_PtrListTestItem(7));

        RS_PtrListTestItem::resetItemDestructorCalled();
        l.replace(0, new RS_PtrListTestItem(2));
        CPPUNIT_ASSERT( RS_PtrListTestItem::itemDestructorCalled()==3 );
        CPPUNIT_ASSERT( l.first()->value()==2 );
        
        RS_PtrListTestItem::resetItemDestructorCalled();
        l.replace(0, new RS_PtrListTestItem(1));
        CPPUNIT_ASSERT( RS_PtrListTestItem::itemDestructorCalled()==2 );
        CPPUNIT_ASSERT( l.first()->value()==1 );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testReplace: 001");
        
        RS_PtrListTestItem::resetItemDestructorCalled();
        l.replace(2, new RS_PtrListTestItem(8));
        CPPUNIT_ASSERT( RS_PtrListTestItem::itemDestructorCalled()==7 );
        CPPUNIT_ASSERT( l[2]->value()==8 );
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "testReplace: 002");
        
        RS_PtrListTestItem::resetItemDestructorCalled();
        l.replace(3, new RS_PtrListTestItem(99));
        CPPUNIT_ASSERT( RS_PtrListTestItem::itemDestructorCalled()==99 );

        RS_DEBUG->print(RS_Debug::D_TESTING, "testReplace: OK");
    }
    
};


