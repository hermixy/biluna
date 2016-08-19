#include <stdio.h>
#include <cppunit/extensions/HelperMacros.h>
#include "rs_layerlist.h"
#include "qg_actionhandler.h"
#include "qg_layerwidget.h"
#include "qg_mainwindowinterfacetest.h"

class QG_LayerWidgetTest : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( QG_LayerWidgetTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();

private:
    
public:
    void setUp() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_LayerWidgetTest");
    }

    void tearDown() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "RS_LayerWidgetTest: OK");
    }
    
    void test() {
        RS_DEBUG->print(RS_Debug::D_TESTING, "test");

        // init:
        QG_MainWindowInterfaceTest mainWindow;
        QG_LayerWidget* layerWidget = 
            new QG_LayerWidget(mainWindow.getActionHandler(), NULL);
        RS_LayerList* layerList = new RS_LayerList();
        layerWidget->setLayerList(layerList, true);
        layerList->addListener(layerWidget);

        // act:
        layerList->add(new RS_Layer(RS_LayerData("layer1", 
                                          RS_Pen(RS_Color(255,0,0), 
                                                 RS2::Width00, 
                                                 RS2::SolidLine), 
                                          false, false)));
        layerList->add(new RS_Layer(RS_LayerData("layer2", 
                                          RS_Pen(RS_Color(0,0,0), 
                                                 RS2::Width00, 
                                                 RS2::SolidLine), 
                                          false, false)));

        // verify:
        QListWidget* listWidget = layerWidget->getListBox();
        CPPUNIT_ASSERT(listWidget!=NULL);
        RS_DEBUG->print(RS_Debug::D_TESTING, "test: count: %d", listWidget->count());
        for (int i=0; i<listWidget->count(); ++i) {
            QListWidgetItem* item = listWidget->item(i);
            RS_DEBUG->print(RS_Debug::D_TESTING, "test: item : %s", (const char*)item->text().toLatin1());
        }
        CPPUNIT_ASSERT(listWidget->count()==2);

        /*
        layerWidget->show();
        qApp->exec();
        */

        delete layerWidget;
        delete layerList;
        
        RS_DEBUG->print(RS_Debug::D_TESTING, "test: OK");
    }
};
