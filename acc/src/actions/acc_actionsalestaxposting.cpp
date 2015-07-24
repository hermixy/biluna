/*****************************************************************
 * $Id: acc_actionsalestaxposting.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 19, 2012 4:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionsalestaxposting.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"


ACC_ActionSalesTaxPosting::ACC_ActionSalesTaxPosting()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionSalesTaxPosting::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionSalesTaxPosting::getName(),
                              //QIcon(":/images/icons/star.png"),
                              tr("&Sales Tax Posting"));
        ga->setStatusTip(tr("Sales GL postings maintenance"));
        ga->setFactory(ACC_ActionSalesTaxPosting::factory);
    }
    return ga;
}

RB_Action* ACC_ActionSalesTaxPosting::factory() {
    RB_Action* a = new ACC_ActionSalesTaxPosting();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionSalesTaxPosting::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogSalesTaxPosting);
    dlg->exec();
    dlg->deleteLater();
}
