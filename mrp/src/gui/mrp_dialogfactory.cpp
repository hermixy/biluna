/*****************************************************************
 * $Id: sail_dialogfactory.cpp 1979 2013-08-22 19:56:38Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "mrp_dialogfactory.h"

#include "db_dialogfactory.h"
#include "mrp_selectcontactdialog.h"
#include "mrp_selectcustomerdialog.h"
#include "mrp_selectorderdialog.h"
#include "mrp_selectprojectdialog.h"
#include "mrp_softwarelicensereportwidget.h"
#include "mrp_softwarelicensewidget.h"
#include "rb_dockwidget.h"
#include "rb_mainwindow.h"
#include "rb_mdiwindow.h"

MRP_DialogFactory* MRP_DialogFactory::mActiveFactory = 0;


/**
 * Constructor
 */
MRP_DialogFactory::MRP_DialogFactory(RB_MainWindow* mw)
        : RB_DialogFactory(mw) {
    RB_DEBUG->print("MRP_DialogFactory::MRP_DialogFactory()");

    this->setPerspective(RB2::PerspectiveMRP);
    DB_DIALOGFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
MRP_DialogFactory::~MRP_DialogFactory() {
    DB_DIALOGFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("MRP_DialogFactory::~MRP_DialogFactory() OK");
}

/**
 * Get unique (static) instance of dialog factory
 */
MRP_DialogFactory* MRP_DialogFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new MRP_DialogFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @return DB model factory
 */
RB_ModelFactory* MRP_DialogFactory::getModelFactory() {
    return MRP_MODELFACTORY;
}

/**
 * Get or create widget for dock- or MDI window,
 * could also be for dialog or mainwindow
 * @param type type of widget
 * @param parent parent for constructor of widget
 * @return widget
 */
RB_Widget* MRP_DialogFactory::getWidget(int type, QWidget* parent) {
    RB_Widget* wgt = findWidget(type);
    if (wgt) return wgt; // all widgets are unique

    switch (type) {
    case WidgetSoftwareLicense: {
        wgt = new MRP_SoftwareLicenseWidget(parent);
        break;
    }
    case WidgetSoftwareLicenseReport: {
        wgt = new MRP_SoftwareLicenseReportWidget(parent);
        break;
    }
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "MRP_DialogFactory::getWidget() "
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
RB_Dialog* MRP_DialogFactory::getDialog(int type) {
    RB_DEBUG->print("MRP_DialogFactory::getDialog()");

    RB_Dialog* dlg = NULL;

    switch (type) {
    case DialogSelectContact: {
        dlg = new MRP_SelectContactDialog(mMainWindow);
        break;
    }
    case DialogSelectCustomer: {
        dlg = new MRP_SelectCustomerDialog(mMainWindow);
        break;
    }
    case DialogSelectOrder: {
        dlg = new MRP_SelectOrderDialog(mMainWindow);
        break;
    }
    case DialogSelectProject : {
        dlg = new MRP_SelectProjectDialog(mMainWindow);
        break;
    }
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "MRP_DialogFactory::getDialog(type)"
                        "non-existing dialog requested ERROR");
        requestWarningDialog("MRP_DialogFactory::getDialog() ERROR\n"
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
void MRP_DialogFactory::updateUiConnections() {
    RB_DEBUG->print("MRP_DialogFactory::updateUiConnections()");

    // For dock windows

}

/**
 * Called when an action needs to communicate 'message' to the user.
 */
void MRP_DialogFactory::commandMessage(const RB_String& message) {
    DB_DIALOGFACTORY->commandMessage(message);
}
