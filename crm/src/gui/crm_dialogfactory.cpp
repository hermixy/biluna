/*****************************************************************
 * $Id: crm_dialogfactory.cpp 2076 2014-01-27 16:02:14Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_dialogfactory.h"

#include "crm_activityreportwidget.h"
#include "crm_activitywidget.h"
#include "crm_campaigntargetwidget.h"
#include "crm_contactbycustreportwidget.h"
#include "crm_contactreportwidget.h"
#include "crm_contactwidget.h"
#include "crm_customerreportwidget.h"
#include "crm_customerwidget.h"
#include "crm_documentwidget.h"
#include "crm_groupcontactwidget.h"
#include "crm_leadsourcetypedialog.h"
#include "crm_projectdialog.h"
#include "crm_projecteditwidget.h"
#include "crm_selectcontactdialog.h"
#include "crm_selectcustomerdialog.h"
#include "crm_selectgroupdialog.h"
#include "crm_selectsystemuserdialog.h"
#include "crm_selecttemplatedialog.h"
#include "crm_sysseqnodialog.h"
#include "crm_templatewidget.h"
#include "db_dialogfactory.h"
#include "rb_dockwidget.h"
#include "rb_mainwindow.h"
#include "rb_mdiwindow.h"

CRM_DialogFactory* CRM_DialogFactory::mActiveFactory = 0;

/**
 * Constructor
 */
CRM_DialogFactory::CRM_DialogFactory(RB_MainWindow* mw)
        : RB_DialogFactory(mw) {
    RB_DEBUG->print("CRM_DialogFactory::CRM_DialogFactory()");

    this->setPerspective(RB2::PerspectiveCRM);
    DB_DIALOGFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
CRM_DialogFactory::~CRM_DialogFactory() {
    DB_DIALOGFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("CRM_DialogFactory::~CRM_DialogFactory() OK");
}

/**
 * Get unique (static) instance of dialog factory
 */
CRM_DialogFactory* CRM_DialogFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new CRM_DialogFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @return DB model factory
 */
RB_ModelFactory* CRM_DialogFactory::getModelFactory() {
    return CRM_MODELFACTORY;
}

/**
 * Get or create widget for dock- or MDI window,
 * could also be for dialog or mainwindow
 * @param type type of widget
 * @param parent parent for constructor of widget
 * @return widget
 */
RB_Widget* CRM_DialogFactory::getWidget(int type, QWidget* parent) {
    RB_Widget* wgt = findWidget(type);
    if (wgt) return wgt; // all widgets are unique

    switch (type) {
    case WidgetActivity: {
        wgt = new CRM_ActivityWidget(parent);
        break;
    }
    case WidgetActivityReport: {
        wgt = new CRM_ActivityReportWidget(parent);
        break;
    }
    case WidgetCampaignTarget: {
        wgt = new CRM_CampaignTargetWidget(parent);
        break;
    }
    case WidgetContact: {
        wgt = new CRM_ContactWidget(parent);
        break;
    }
    case WidgetContactByCustReport: {
        wgt = new CRM_ContactByCustReportWidget(parent);
        break;
    }
    case WidgetContactReport: {
        wgt = new CRM_ContactReportWidget(parent);
        break;
    }
    case WidgetCustomer: {
        wgt = new CRM_CustomerWidget(parent);
        break;
    }
    case WidgetCustomerReport: {
        wgt = new CRM_CustomerReportWidget(parent);
        break;

    }
    case WidgetDocument: {
        wgt = new CRM_DocumentWidget(parent);
        break;
    }
    case WidgetGroupContact: {
        wgt = new CRM_GroupContactWidget(parent);
        break;

    }
    case WidgetProjectEdit: {
        wgt = new CRM_ProjectEditWidget(parent);
        break;

    }
    case WidgetTemplate: {
        wgt = new CRM_TemplateWidget(parent);
        break;
    }
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "CRM_DialogFactory::getWidget() "
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
RB_Dialog* CRM_DialogFactory::getDialog(int type) {
    RB_DEBUG->print("CRM_DialogFactory::getDialog()");

    RB_Dialog* dlg = NULL;

    switch (type) {
    case DialogLeadSourceType : {
        dlg = new CRM_LeadSourceTypeDialog(mMainWindow);
        break;
    }
    case DialogProject: {
        dlg = new CRM_ProjectDialog(mMainWindow);
        break;
    }
    case DialogSelectContact: {
        dlg = new CRM_SelectContactDialog(mMainWindow);
        break;
    }
    case DialogSelectCustomer: {
        dlg = new CRM_SelectCustomerDialog(mMainWindow);
        break;
    }
    case DialogSelectGroup: {
        dlg = new CRM_SelectGroupDialog(mMainWindow);
        break;
    }
    case DialogSelectSystemUser: {
        dlg = new CRM_SelectSystemUserDialog(mMainWindow);
        break;
    }
    case DialogSelectTemplate: {
        dlg = new CRM_SelectTemplateDialog(mMainWindow);
        break;
    }
    case DialogSysSeqNo: {
        dlg = new CRM_SysSeqNoDialog(mMainWindow);
        break;
    }
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "CRM_DialogFactory::getDialog(type)"
                        "non-existing dialog requested ERROR");
        requestWarningDialog("CRM_DialogFactory::getDialog() ERROR\n"
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
void CRM_DialogFactory::updateUiConnections() {
    RB_DEBUG->print("CRM_DialogFactory::updateUiConnections()");

    // For dock windows

}

/**
 * Called when an action needs to communicate 'message' to the user.
 */
void CRM_DialogFactory::commandMessage(const RB_String& message) {
    DB_DIALOGFACTORY->commandMessage(message);
}
