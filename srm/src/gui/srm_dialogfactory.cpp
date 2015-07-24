/*****************************************************************
 * $Id: srm_dialogfactory.cpp 2076 2014-01-27 16:02:14Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_dialogfactory.h"

#include "srm_activityreportwidget.h"
#include "srm_activitywidget.h"
#include "srm_campaigntargetwidget.h"
#include "srm_contactbysuppreportwidget.h"
#include "srm_contactreportwidget.h"
#include "srm_contactwidget.h"
#include "srm_documentwidget.h"
#include "srm_groupcontactwidget.h"
#include "srm_leadsourcetypedialog.h"
#include "srm_projectdialog.h"
#include "srm_projecteditdialog.h"
#include "srm_selectcontactdialog.h"
#include "srm_selectsupplierdialog.h"
//#include "srm_selectdocumentdialog.h"
#include "srm_selectgroupdialog.h"
#include "srm_selecttemplatedialog.h"
#include "srm_supplierreportwidget.h"
#include "srm_supplierwidget.h"
#include "srm_sysseqnodialog.h"
#include "srm_templatewidget.h"
#include "db_dialogfactory.h"
#include "rb_dockwidget.h"
#include "rb_mainwindow.h"
#include "rb_mdiwindow.h"

SRM_DialogFactory* SRM_DialogFactory::mActiveFactory = 0;

/**
 * Constructor
 */
SRM_DialogFactory::SRM_DialogFactory(RB_MainWindow* mw)
        : RB_DialogFactory(mw) {
    RB_DEBUG->print("SRM_DialogFactory::SRM_DialogFactory()");

    this->setPerspective(RB2::PerspectiveSRM);
    DB_DIALOGFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
SRM_DialogFactory::~SRM_DialogFactory() {
    DB_DIALOGFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("SRM_DialogFactory::~SRM_DialogFactory() OK");
}

/**
 * Get unique (static) instance of dialog factory
 */
SRM_DialogFactory* SRM_DialogFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new SRM_DialogFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @return DB model factory
 */
RB_ModelFactory* SRM_DialogFactory::getModelFactory() {
    return SRM_MODELFACTORY;
}

/**
 * Get or create widget for dock- or MDI window,
 * could also be for dialog or mainwindow
 * @param type type of widget
 * @param parent parent for constructor of widget
 * @return widget
 */
RB_Widget* SRM_DialogFactory::getWidget(int type, QWidget* parent) {
    RB_Widget* wgt = findWidget(type);
    if (wgt) return wgt; // all widgets are unique

    switch (type) {
    case WidgetActivity: {
        wgt = new SRM_ActivityWidget(parent);
        break;
    }
    case WidgetActivityReport: {
        wgt = new SRM_ActivityReportWidget(parent);
        break;
    }
    case WidgetCampaignTarget: {
        wgt = new SRM_CampaignTargetWidget(parent);
        break;
    }
    case WidgetContact: {
        wgt = new SRM_ContactWidget(parent);
        break;
    }
    case WidgetContactBySuppReport: {
        wgt = new SRM_ContactBySuppReportWidget(parent);
        break;
    }
    case WidgetContactReport: {
        wgt = new SRM_ContactReportWidget(parent);
        break;
    }
    case WidgetDocument: {
        wgt = new SRM_DocumentWidget(parent);
        break;
    }
    case WidgetGroupContact: {
        wgt = new SRM_GroupContactWidget(parent);
        break;
    }
    case WidgetSupplier: {
        wgt = new SRM_SupplierWidget(parent);
        break;
    }
    case WidgetSupplierReport: {
        wgt = new SRM_SupplierReportWidget(parent);
        break;
    }
    case WidgetTemplate: {
        wgt = new SRM_TemplateWidget(parent);
        break;
    }
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "SRM_DialogFactory::getWidget() "
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
RB_Dialog* SRM_DialogFactory::getDialog(int type) {
    RB_DEBUG->print("SRM_DialogFactory::getDialog()");

    RB_Dialog* dlg = NULL;

    switch (type) {
    case DialogLeadSourceType : {
        dlg = new SRM_LeadSourceTypeDialog(mMainWindow);
        break;
    }
    case DialogProject: {
        dlg = new SRM_ProjectDialog(mMainWindow);
        break;
    }
    case DialogProjectEdit: {
        dlg = new SRM_ProjectEditDialog(mMainWindow);
        break;
    }
    case DialogSelectContact: {
        dlg = new SRM_SelectContactDialog(mMainWindow);
        break;
    }
    case DialogSelectGroup: {
        dlg = new SRM_SelectGroupDialog(mMainWindow);
        break;
    }
    case DialogSelectSupplier: {
        dlg = new SRM_SelectSupplierDialog(mMainWindow);
        break;
    }
    case DialogSelectTemplate: {
        dlg = new SRM_SelectTemplateDialog(mMainWindow);
        break;
    }
    case DialogSysSeqNo: {
        dlg = new SRM_SysSeqNoDialog(mMainWindow);
        break;
    }
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "SRM_DialogFactory::getDialog(type)"
                        "non-existing dialog requested ERROR");
        requestWarningDialog("SRM_DialogFactory::getDialog() ERROR\n"
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
void SRM_DialogFactory::updateUiConnections() {
    RB_DEBUG->print("SRM_DialogFactory::updateUiConnections()");

    // For dock windows

}

/**
 * Called when an action needs to communicate 'message' to the user.
 */
void SRM_DialogFactory::commandMessage(const RB_String& message) {
    DB_DIALOGFACTORY->commandMessage(message);
}
