/*****************************************************************
 * $Id: sail_dialogfactory.cpp 1979 2013-08-22 19:56:38Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_dialogfactory.h"

#include "sail_mapnavigationwidget.h"
#include "sail_polardiagramwidget.h"
#include "sail_projectdialog.h"
#include "sail_projecteditwidget.h"
#include "sail_selectcoordinatedialog.h"
#include "sail_selectmapdialog.h"
#include "sail_selectsymboldialog.h"
#include "sail_trackdistancereportwidget.h"
#include "db_dialogfactory.h"
#include "rb_dockwidget.h"
#include "rb_mainwindow.h"
#include "rb_mdiwindow.h"

SAIL_DialogFactory* SAIL_DialogFactory::mActiveFactory = 0;

/**
 * Constructor
 */
SAIL_DialogFactory::SAIL_DialogFactory(RB_MainWindow* mw)
        : RB_DialogFactory(mw) {
    RB_DEBUG->print("SAIL_DialogFactory::SAIL_DialogFactory()");

    this->setPerspective(RB2::PerspectiveSAIL);
    DB_DIALOGFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
SAIL_DialogFactory::~SAIL_DialogFactory() {
    DB_DIALOGFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("SAIL_DialogFactory::~SAIL_DialogFactory() OK");
}

/**
 * Get unique (static) instance of dialog factory
 */
SAIL_DialogFactory* SAIL_DialogFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new SAIL_DialogFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @return DB model factory
 */
RB_ModelFactory* SAIL_DialogFactory::getModelFactory() {
    return SAIL_MODELFACTORY;
}

/**
 * Get or create widget for dock- or MDI window,
 * could also be for dialog or mainwindow
 * @param type type of widget
 * @param parent parent for constructor of widget
 * @return widget
 */
RB_Widget* SAIL_DialogFactory::getWidget(int type, QWidget* parent) {
    RB_Widget* wgt = findWidget(type);
    if (wgt) return wgt; // all widgets are unique

    switch (type) {
    case WidgetMapNavigation: {
        wgt = new SAIL_MapNavigationWidget(parent);
        break;
    }
    case WidgetPolarDiagram: {
        wgt = new SAIL_PolarDiagramWidget(parent);
        break;
    }
    case WidgetProjectEdit : {
        wgt = new SAIL_ProjectEditWidget(parent);
        break;
    }
    case WidgetTrackDistanceReport: {
        wgt = new SAIL_TrackDistanceReportWidget(parent);
        break;
    }
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "SAIL_DialogFactory::getWidget() "
                        "non-existing widget requested ERROR");
        break;
    }

    if (wgt) {
        wgt->setWidgetType(type);
        wgt->setDialogFactory(this);
        // Set all widgets as unique widget, set 'inactive' via removeWidget()
        mWidgetList[type] = wgt;
        // not required here wgt->init();
    }

    return wgt;
}

/**
 * Request a dialog
 * @param type type of dialog
 * @return Dialog
 */
RB_Dialog* SAIL_DialogFactory::getDialog(int type) {
    RB_DEBUG->print("SAIL_DialogFactory::getDialog()");

    RB_Dialog* dlg = NULL;

    switch (type) {
    case DialogProject : {
        dlg = new SAIL_ProjectDialog(mMainWindow);
        break;
    }
    case DialogSelectCoordinate: {
        dlg = new SAIL_SelectCoordinateDialog(mMainWindow);
        break;
    }
    case DialogSelectMap: {
        dlg = new SAIL_SelectMapDialog(mMainWindow);
        break;
    }
    case DialogSelectSymbol: {
        dlg = new SAIL_SelectSymbolDialog(mMainWindow);
        break;
    }
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "SAIL_DialogFactory::getDialog(type)"
                        "non-existing dialog requested ERROR");
        requestWarningDialog("SAIL_DialogFactory::getDialog() ERROR\n"
                             "Non-existing dialog requested.");
        break;
    }

    if (dlg) {
        dlg->init();
    }
    return dlg;
}

/**
 * Update signal slot user interface connections
 */
void SAIL_DialogFactory::updateUiConnections() {
    RB_DEBUG->print("SAIL_DialogFactory::updateUiConnections()");

    // For dock windows

}

/**
 * Called when an action needs to communicate 'message' to the user.
 */
void SAIL_DialogFactory::commandMessage(const RB_String& message) {
    DB_DIALOGFACTORY->commandMessage(message);
}
