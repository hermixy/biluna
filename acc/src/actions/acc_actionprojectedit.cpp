/*****************************************************************
 * $Id: acc_actionprojectedit.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Mar 24, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionprojectedit.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_utilityfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionProjectEdit::ACC_ActionProjectEdit()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionProjectEdit::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionProjectEdit::getName(),
                              //QIcon(":/images/icons/star.png"),
                              tr("&Company Edit"));
        ga->setStatusTip(tr("Company settings maintenance"));
        ga->setFactory(ACC_ActionProjectEdit::factory);
    }
    return ga;
}

RB_Action* ACC_ActionProjectEdit::factory() {
    RB_Action* a = new ACC_ActionProjectEdit();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionProjectEdit::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogProjectEdit);

    if (dlg->exec() == QDialog::Accepted) {
        // Make sure current company object (ACC_Project) is up-to-date
        RB_ObjectBase* root = ACC_MODELFACTORY->getRoot();

        if (!root) {
            RB_DEBUG->print(RB_Debug::D_ERROR,
                            "ACC_ActionProjectEdit::trigger() "
                            "root is NULL ERROR");
        } else {
            root->dbRead(ACC_MODELFACTORY->getDatabase(), RB2::ResolveNone);
            // Refresh all utility factories
            ACC_UTILITYFACTORY->refresh();
        }
    }

    dlg->deleteLater();
}
