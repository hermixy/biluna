#ifndef QG_MAINWINDOWINTERFACETEST
#define QG_MAINWINDOWINTERFACETEST

#include "qg_mainwindowinterface.h"
#include "rs_bitmapgraphicview.h"
#include "rs_graphic.h"


class QG_MainWindowInterfaceTest : public QG_MainWindowInterface {
public:
    QG_MainWindowInterfaceTest() {
        graphicView = new RS_BitmapGraphicView(250, 250);
        document = new RS_Graphic();
        actionHandler = new QG_ActionHandler(this);
    }
    
    virtual RS_GraphicView* getGraphicView() {
        return graphicView;
    }
    
    virtual RS_Document* getDocument() {
        return document;
    }

	virtual void createNewDocument(
		const RS_String& fileName = RS_String::null, RS_Document* doc=NULL) {
        assert(false);
    }

    virtual void loadDocument(const RS_String&) {}
    virtual void closeDocument() {}
    
	virtual void showSimulationControls() {
    }

    virtual QMainWindow* getMainWindow() {
        assert(false);
        return NULL;
    }
    
    virtual QG_ActionHandler* getActionHandler() {
        return actionHandler;
    }
    
	virtual void setFocus2() {
    }

private:
    QG_ActionHandler* actionHandler;
    RS_BitmapGraphicView* graphicView;
    RS_Document* document;
};

#endif
