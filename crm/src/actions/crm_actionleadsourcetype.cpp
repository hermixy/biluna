/*****************************************************************
 * $Id: crm_actionleadsourcetype.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Mar 3, 2012 15:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_actionleadsourcetype.h"

#include "crm_dialogfactory.h"
#include "crm_projectdialog.h"
#include "db_modelfactory.h"
#include "rb_database.h"
#include "rb_mainwindow.h"


CRM_ActionLeadSourceType::CRM_ActionLeadSourceType()
        : RB_Action() {
    // nothing
}


RB_GuiAction* CRM_ActionLeadSourceType::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(CRM_ActionLeadSourceType::getName(),
//                                  "dialog",
                                  tr("&Lead Source Type"));
        ga->setStatusTip(tr("Edit lead source types"));
        ga->setFactory(CRM_ActionLeadSourceType::factory);
    }
    return ga;
}

RB_Action* CRM_ActionLeadSourceType::factory() {
    RB_Action* a = new CRM_ActionLeadSourceType();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void CRM_ActionLeadSourceType::trigger() {
    // Check required settings
    if (CRM_MODELFACTORY->getRootId() == ""
            || !CRM_MODELFACTORY->getDatabase().isOpen()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = CRM_DIALOGFACTORY->getDialog(CRM_DialogFactory::DialogLeadSourceType);
    dlg->exec();
    dlg->deleteLater();
}
