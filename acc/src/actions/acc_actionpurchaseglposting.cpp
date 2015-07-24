/*****************************************************************
 * $Id: acc_actionpurchaseglposting.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 28, 2013 17:49:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionpurchaseglposting.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"


ACC_ActionPurchaseGlPosting::ACC_ActionPurchaseGlPosting()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionPurchaseGlPosting::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionPurchaseGlPosting::getName(),
                              //QIcon(":/images/icons/star.png"),
                              tr("&Purchase GL Postings"));
        ga->setStatusTip(tr("Purchase GL postings maintenance"));
        ga->setFactory(ACC_ActionPurchaseGlPosting::factory);
    }
    return ga;
}

RB_Action* ACC_ActionPurchaseGlPosting::factory() {
    RB_Action* a = new ACC_ActionPurchaseGlPosting();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionPurchaseGlPosting::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogPurchaseGlPosting);
    dlg->exec();
    dlg->deleteLater();
}
