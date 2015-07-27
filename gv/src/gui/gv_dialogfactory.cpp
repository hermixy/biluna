/*****************************************************************
 * $Id: gv_dialogfactory.cpp 1479 2011-11-17 15:51:46Z rutger $
 * Created: Sep 29, 2008 4:07:19 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_dialogfactory.h"

#include "db_dialogfactory.h"
#include "gv_drawinglistwidget.h"
#include "gv_drawingwidget.h"

#include "gv_graphicsviewqtwidget.h"
#include "gv_graphicsviewqvwidget.h"

#include "gv_legendwidget.h"
#include "gv_lineattributesdialog.h"
#include "gv_mainwindow.h"
#include "gv_onetimedialog.h"
//#include "gv_pipelinedialog.h"

#include "gv_previewwidget.h"
#include "gv_projectdialog.h"
#include "gv_propertieswidget.h"

#include "rb_mainwindow.h"
#include "rb_mdiwindow.h"

#include "gv_drawing.h"
#include "gv_graphicsview.h"
#include "gv_graphicsscene.h"
#include "gv_snapper.h"


GV_DialogFactory* GV_DialogFactory::mActiveFactory = 0;

/**
 * Constructor
 */
GV_DialogFactory::GV_DialogFactory(RB_MainWindow* mw)
        : RB_DialogFactory(mw) {
    RB_DEBUG->print("GV_DialogFactory::GV_DialogFactory()");
    this->setPerspective(RB2::PerspectiveGV);
    DB_DIALOGFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
GV_DialogFactory::~GV_DialogFactory() {
    DB_DIALOGFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("GV_DialogFactory::~GV_DialogFactory() OK");
}

/**
 * Get unique (static) instance of dialog factory
 */
GV_DialogFactory* GV_DialogFactory::getInstance(RB_MainWindow* mw) {
    RB_DEBUG->print("GV_DialogFactory::getInstance()");

    if (!mActiveFactory) {
        mActiveFactory = new GV_DialogFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @return DB model factory
 */
RB_ModelFactory* GV_DialogFactory::getModelFactory() {
    return GV_MODELFACTORY;
}

/**
 * Create widget for dock- or MDI window, could also be for dialog or mainwindow
 * @param type type of widget
 * @param parent parent for constructor of widget
 * @return widget
 */
RB_Widget* GV_DialogFactory::getWidget(int type, QWidget* parent) {
    RB_Widget* wgt = findWidget(type);
    if (wgt) return wgt; // widget already found

    switch (type) {
    case WidgetDrawing: {
//        QDockWidget* dw = getDockWidget(GV_DialogFactory::WidgetDrawingList,
//                                    "dockwidget_drawinglist",
//                                    Qt::LeftDockWidgetArea);
//        dw->show();

        wgt = new GV_DrawingWidget(parent);
        break;
    }
    case WidgetDrawingList: {
        // DockWidget drawinglist
        wgt = new GV_DrawingListWidget(parent);
        break;
    }
    case WidgetGraphicsViewQt: {
        wgt = new GV_GraphicsViewQtWidget(parent);
        break;
    }
    case WidgetGraphicsViewQV: {
        wgt = new GV_GraphicsViewQVWidget(parent);
        break;
    }
    case WidgetLegend: {
        // DockWidget legend
        wgt = new GV_LegendWidget(parent);
        break;
    }
    case WidgetPreview: {
        // DockWidget preview
        wgt = new GV_PreviewWidget(parent);
        break;
    }
    case WidgetProperties: {
        // DockWidget properties
        wgt = new GV_PropertiesWidget(parent);
        break;
    }
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "GV_DialogFactory::getWidget() "
                        "non-existing widget requested ERROR");
        break;
    }

    if (wgt) {
        wgt->setWidgetType(type);
        wgt->setDialogFactory(this);
        // Set widget as unique widget, set 'inactive' via removeWidget()
        if (!WidgetDrawing) {
            mWidgetList[type] = wgt;
        }
        // not required here wgt->init();
    }

    return wgt;
}


/**
 * Request a dialog
 * @param type type of dialog
 * @return Dialog
 */
RB_Dialog* GV_DialogFactory::getDialog(int type) {
    RB_DEBUG->print("GV_TestDialogFactory::getDialog()");

    RB_Dialog* dlg = RB_DialogFactory::getDialog(type);
    if (dlg) return dlg; // dialog already found and used

    switch (type) {
    case DialogLineAttributes:
        dlg = new GV_LineAttributesDialog(mMainWindow);
        break;
    case DialogProject: {
        dlg = new GV_ProjectDialog(mMainWindow);
        break;
    }
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "GV_DialogFactory::getDialog(type)"
                        "non-existing dialog requested ERROR");
        break;
    }

    dlg->init();
    return dlg;
}

/**
 * Update signal slot user interface connections
 */
void GV_DialogFactory::updateUiConnections() {
    RB_DEBUG->print("GV_DialogFactory::updateUiConnections()");

    // For dock windows

}

/**
 * @return active drawing
 */
GV_Drawing* GV_DialogFactory::getDrawing() {
    RB_MdiWindow* mdi = dynamic_cast<RB_MdiWindow*>(getActiveMdiWindow());
    if (!mdi) return NULL;
    GV_DrawingWidget* wgt = dynamic_cast<GV_DrawingWidget*>(mdi->getWidget());
    if (!wgt) return NULL;
    return wgt->getDrawing();
}

/**
 * @return active graphicsview
 */
GV_GraphicsView* GV_DialogFactory::getGraphicsView() {
    RB_MdiWindow* mdi = dynamic_cast<RB_MdiWindow*>(getActiveMdiWindow());
    if (!mdi) return NULL;
    GV_DrawingWidget* wgt = dynamic_cast<GV_DrawingWidget*>(mdi->getWidget());
    if (!wgt) return NULL;
    return wgt->getGraphicsView();
}
