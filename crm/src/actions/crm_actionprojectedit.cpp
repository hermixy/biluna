/*****************************************************************
 * $Id: crm_actionprojectedit.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_actionprojectedit.h"

#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"
#include "rb_dialogwindow.h"


CRM_ActionProjectEdit::CRM_ActionProjectEdit()
        : RB_Action() {
    // nothing
}


RB_GuiAction* CRM_ActionProjectEdit::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(CRM_ActionProjectEdit::getName(),
                              //QIcon(":/images/icons/star.png"),
                              tr("&Account Edit"));
        ga->setStatusTip(tr("CRM account settings maintenance"));
        ga->setFactory(CRM_ActionProjectEdit::factory);
    }
    return ga;
}

RB_Action* CRM_ActionProjectEdit::factory() {
    RB_Action* a = new CRM_ActionProjectEdit();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void CRM_ActionProjectEdit::trigger() {
    // Check required settings
    if (CRM_MODELFACTORY->getRootId() == ""
            || !CRM_MODELFACTORY->getDatabase().isOpen()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog, reuse Project dialog for selecting the root account
    RB_DialogWindow* dlg = CRM_DIALOGFACTORY->getDialogWindow(
                CRM_DialogFactory::WidgetProjectEdit);
    dlg->exec();
    dlg->deleteLater();

    // Make sure current company object (CRM_Project) is up-to-date
    RB_ObjectBase* root = CRM_MODELFACTORY->getRoot();

    if (!root) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "CRM_ActionProjectEdit::trigger() "
                        "root is NULL ERROR");
    } else {
        root->dbRead(CRM_MODELFACTORY->getDatabase(), RB2::ResolveNone);
    }
}
