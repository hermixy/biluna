/*****************************************************************
 * $Id: crm_actioncontactbycustreport.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Jun 8, 2012 21:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_actioncontactbycustreport.h"

#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"
#include "db_actionfactory.h"
#include "rb_mdiwindow.h"


CRM_ActionContactByCustReport::CRM_ActionContactByCustReport()
        : RB_Action() {
    // nothing
}


RB_GuiAction* CRM_ActionContactByCustReport::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(CRM_ActionContactByCustReport::getName(),
                              // QIcon(":/images/icons/group_edit_crm.png"),
                              tr("Contact b&y Customer Report"));
        ga->setStatusTip(tr("View contact by customer report"));
        ga->setFactory(CRM_ActionContactByCustReport::factory);
    }
    return ga;
}

RB_Action* CRM_ActionContactByCustReport::factory() {
    RB_Action* a = new CRM_ActionContactByCustReport();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void CRM_ActionContactByCustReport::trigger() {
    // Check required settings
    if (CRM_MODELFACTORY->getRootId() == ""
            || !CRM_MODELFACTORY->getDatabase().isOpen()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Register widget with toolbar
    DB_ACTIONFACTORY->enableEditMenu(CRM_DialogFactory::WidgetContactByCustReport);
    // DB_ACTIONFACTORY->enableFormatMenu(mdiWin->getWidgetType());

    // Show toolbars
    QToolBar* toolBar = CRM_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_edit");
    if (toolBar) {
        toolBar->show();
    }

    // Create widget
    RB_MdiWindow* mdiWin = CRM_DIALOGFACTORY->getMdiWindow(
            CRM_DialogFactory::WidgetContactByCustReport);

    mdiWin->show();
    mdiWin->raise();
}
