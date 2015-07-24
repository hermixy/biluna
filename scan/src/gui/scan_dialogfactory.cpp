/*****************************************************************
 * $Id: scan_dialogfactory.cpp 1849 2012-12-31 11:25:09Z rutger $
 * Created: Apr 26, 2011 11:34:42 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_dialogfactory.h"

#include "db_dialogfactory.h"
#include "rb_dockwidget.h"
#include "rb_mainwindow.h"
#include "rb_mdiwindow.h"
#include "scan_chartwidget.h"
#include "scan_modelfactory.h"
#include "scan_projectdialog.h"
#include "scan_projecteditdialog.h"
#include "scan_scaneditdialog.h"
#include "scan_scanselectdialog.h"
#include "scan_scanresultwidget.h"



SCAN_DialogFactory* SCAN_DialogFactory::mActiveFactory = 0;

/**
 * Constructor
 */
SCAN_DialogFactory::SCAN_DialogFactory(RB_MainWindow* mw)
        : RB_DialogFactory(mw) {
    this->setPerspective(RB2::PerspectiveSCAN);
    DB_DIALOGFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
SCAN_DialogFactory::~SCAN_DialogFactory() {
    DB_DIALOGFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("SCAN_DialogFactory::~SCAN_DialogFactory() OK");
}

/**
 * Get unique (static) instance of dialog factory
 */
SCAN_DialogFactory* SCAN_DialogFactory::getInstance(RB_MainWindow* mw) {
    RB_DEBUG->print("SCAN_DialogFactory::getInstance()");

    if (!mActiveFactory) {
        mActiveFactory = new SCAN_DialogFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @return DB model factory
 */
RB_ModelFactory* SCAN_DialogFactory::getModelFactory() {
    return SCAN_MODELFACTORY;
}

/**
 * Create widget for dock- or MDI window, could also be for dialog or mainwindow
 * @param type type of widget
 * @param parent parent for constructor of widget
 * @return widget
 */
RB_Widget* SCAN_DialogFactory::getWidget(int type, QWidget* parent) {
    RB_Widget* wgt = findWidget(type);

    switch (type) {
    case WidgetReviewChart: {
        if (!wgt) {
            wgt = new SCAN_ChartWidget(parent);
        }
        break;
    }
    case WidgetScanResult: {
        if (!wgt) {
            wgt = new SCAN_ScanResultWidget(parent);
        }
        break;
    }
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "SCAN_DialogFactory::getWidget() "
                        "non-existing widget requested ERROR");
        break;
    }

    if (wgt) {
        wgt->setWidgetType(type);
        wgt->setDialogFactory(this);
        // Set widget as unique widget, set 'inactive' via removeWidget()
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
RB_Dialog* SCAN_DialogFactory::getDialog(int type) {
    RB_DEBUG->print("SCAN_TestDialogFactory::getDialog()");

    RB_Dialog* dlg = NULL;

    switch (type) {
    case DialogProject: {
        dlg = new SCAN_ProjectDialog(mMainWindow);
        break;
    }
    case DialogProjectEdit: {
        dlg = new SCAN_ProjectEditDialog(mMainWindow);
        break;
    }
    case DialogScanEdit: {
        dlg = new SCAN_ScanEditDialog(mMainWindow);
        break;
    }
    case DialogScanSelect: {
        dlg = new SCAN_ScanSelectDialog(mMainWindow);
        break;
    }
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "SCAN_DialogFactory::getDialog(type)"
                        "non-existing dialog requested ERROR");
        requestWarningDialog("SCAN_DialogFactory::getDialog() ERROR\n"
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
void SCAN_DialogFactory::updateUiConnections() {
    RB_DEBUG->print("SCAN_DialogFactory::updateUiConnections()");

    // For dock windows, still valid?

}
