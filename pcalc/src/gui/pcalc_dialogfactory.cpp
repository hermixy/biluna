/*****************************************************************
 * $Id: pcalc_dialogfactory.cpp 2165 2014-09-24 19:22:36Z rutger $
 * Created: Apr 29, 2015 11:34:42 AM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_dialogfactory.h"

#include "db_dialogfactory.h"
#include "pcalc_en1591widget.h"
#include "pcalc_en13555gasketwidget.h"
#include "pcalc_en13555manufwidget.h"
#include "pcalc_en13555typewidget.h"
#include "pcalc_en1591selectgasketwidget.h"
#include "pcalc_modelfactory.h"
#include "pcalc_projectdialog.h"
#include "pcalc_projecteditdialog.h"
#include "rb_dockwidget.h"
#include "rb_mainwindow.h"
#include "rb_mdiwindow.h"


PCALC_DialogFactory* PCALC_DialogFactory::mActiveFactory = 0;

/**
 * Constructor
 */
PCALC_DialogFactory::PCALC_DialogFactory(RB_MainWindow* mw)
        : RB_DialogFactory(mw) {
    this->setPerspective(RB2::PerspectivePCALC);
    DB_DIALOGFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
PCALC_DialogFactory::~PCALC_DialogFactory() {
    DB_DIALOGFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("PCALC_DialogFactory::~PCALC_DialogFactory() OK");
}

/**
 * Get unique (static) instance of dialog factory
 */
PCALC_DialogFactory* PCALC_DialogFactory::getInstance(RB_MainWindow* mw) {
    RB_DEBUG->print("PCALC_DialogFactory::getInstance()");

    if (!mActiveFactory) {
        mActiveFactory = new PCALC_DialogFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @return DB model factory
 */
RB_ModelFactory* PCALC_DialogFactory::getModelFactory() {
    return PCALC_MODELFACTORY;
}

/**
 * Create widget for dock- or MDI window, could also be for dialog or mainwindow
 * @param type type of widget
 * @param parent parent for constructor of widget
 * @return widget
 */
RB_Widget* PCALC_DialogFactory::getWidget(int type, QWidget* parent) {
    RB_Widget* wgt = findWidget(type);
    if (wgt) return wgt; // widget already found

    switch (type) {
    case WidgetEN13555Gasket: {
        wgt = new PCALC_EN13555GasketWidget(parent);
        break;
    }
    case WidgetEN13555Manuf: {
        wgt = new PCALC_EN13555ManufWidget(parent);
        break;
    }
    case WidgetEN13555Type: {
        wgt = new PCALC_EN13555TypeWidget(parent);
        break;
    }
    case WidgetEN1591Flange: {
        wgt = new PCALC_EN1591Widget(parent);
        break;
    }
    case WidgetEN1591SelectGasket: {
        wgt = new PCALC_EN1591SelectGasketWidget(parent);
        break;
    }
    default:
        RB_DEBUG->error("PCALC_DialogFactory::getWidget() "
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
RB_Dialog* PCALC_DialogFactory::getDialog(int type) {
    RB_DEBUG->print("PCALC_TestDialogFactory::getDialog()");

    RB_Dialog* dlg = NULL;

    switch (type) {
    case DialogProject: {
        dlg = new PCALC_ProjectDialog(mMainWindow);
        break;
    }
    case DialogProjectEdit: {
        dlg = new PCALC_ProjectEditDialog(mMainWindow);
        break;
    }
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "PCALC_DialogFactory::getDialog(type)"
                        "non-existing dialog requested ERROR");
        requestWarningDialog("PCALC_DialogFactory::getDialog() ERROR\n"
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
void PCALC_DialogFactory::updateUiConnections() {
    RB_DEBUG->print("PCALC_DialogFactory::updateUiConnections()");

    // For dock windows

}

/**
 * Called when an action needs to communicate 'message' to the user.
 */
void PCALC_DialogFactory::commandMessage(const RB_String& message) {
    DB_DIALOGFACTORY->commandMessage(message);
}

