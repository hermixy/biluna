/*****************************************************************
 * $Id: acc_actionholdreason.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Febn 18, 2010 8:14:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionholdreason.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionHoldReason::ACC_ActionHoldReason()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionHoldReason::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionHoldReason::getName(),
                              QIcon(":/images/icons/group_dollar.png"),
                              tr("C&redit Status"));
        ga->setStatusTip(tr("Customer credit status or hold reasons maintenance"));
        ga->setFactory(ACC_ActionHoldReason::factory);
    }
    return ga;
}

RB_Action* ACC_ActionHoldReason::factory() {
    RB_Action* a = new ACC_ActionHoldReason();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionHoldReason::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogHoldReason);
    dlg->exec();
    dlg->deleteLater();
}
