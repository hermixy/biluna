#include <stdio.h>
#include <cppunit/extensions/HelperMacros.h>
#include "qg_actionhandler.h"
#include "qg_blockwidget.h"
#include "qg_mainwindowinterfacetest.h"

class QG_BlockWidgetTest : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( QG_BlockWidgetTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_BlockWidgetTest");
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_BlockWidgetTest: OK");
    }
    
    void test() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "test");

        // init:
        QG_MainWindowInterfaceTest mainWindow;
        QG_BlockWidget* blockWidget = new QG_BlockWidget(mainWindow.getActionHandler(), NULL);
        RS_BlockList* blockList = new RS_BlockList(true);
        blockWidget->setBlockList(blockList);
        blockList->addListener(blockWidget);

        // act:
        blockList->add(new RS_Block(NULL, RS_BlockData("block1", RS_Vector(0,0), false)));
        blockList->add(new RS_Block(NULL, RS_BlockData("block2", RS_Vector(0,0), false)));

        // verify:
        QListWidget* listWidget = blockWidget->getListBox();
        CPPUNIT_ASSERT(listWidget!=NULL);
        RS_DEBUG->print(RS_Debug::D_TESTING, "test: count: %d", listWidget->count());
        for (int i=0; i<listWidget->count(); ++i) {
            QListWidgetItem* item = listWidget->item(i);
            RS_DEBUG->print(RS_Debug::D_TESTING, "test: item : %s", (const char*)item->text().toLatin1());
        }
        CPPUNIT_ASSERT(listWidget->count()==2);

        //blockWidget->show();
        //qApp->exec();

        delete blockWidget;
        delete blockList;
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "test: OK");
    }
};
