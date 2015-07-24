/*****************************************************************
 * $Id: crm_actionsysseqno.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 26, 2014 10:51:42 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_actionsysseqno.h"

#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"
#include "rb_mdiwindow.h"


CRM_ActionSysSeqNo::CRM_ActionSysSeqNo()
        : RB_Action() {
    // nothing
}


RB_GuiAction* CRM_ActionSysSeqNo::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(CRM_ActionSysSeqNo::getName(),
                              //QIcon(":/images/icons/star.png"),
                              tr("S&ystem Sequence Numbers"));
        ga->setStatusTip(tr("System sequence number maintenance"));
        ga->setFactory(CRM_ActionSysSeqNo::factory);
    }
    return ga;
}

RB_Action* CRM_ActionSysSeqNo::factory() {
    RB_Action* a = new CRM_ActionSysSeqNo();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void CRM_ActionSysSeqNo::trigger() {
    // Check required settings
    if (CRM_MODELFACTORY->getRootId() == ""
            || !CRM_MODELFACTORY->getDatabase().isOpen()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = CRM_DIALOGFACTORY->getDialog(CRM_DialogFactory::DialogSysSeqNo);
    dlg->exec();
    dlg->deleteLater();
}
