/*****************************************************************
 * $Id: acc_actionsalesglposting.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Apr 05, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionsalesglposting.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"


ACC_ActionSalesGlPosting::ACC_ActionSalesGlPosting()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionSalesGlPosting::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionSalesGlPosting::getName(),
                              //QIcon(":/images/icons/star.png"),
                              tr("&Sales GL Postings"));
        ga->setStatusTip(tr("Sales GL postings maintenance"));
        ga->setFactory(ACC_ActionSalesGlPosting::factory);
    }
    return ga;
}

RB_Action* ACC_ActionSalesGlPosting::factory() {
    RB_Action* a = new ACC_ActionSalesGlPosting();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionSalesGlPosting::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogSalesGlPosting);
    dlg->exec();
    dlg->deleteLater();
}
