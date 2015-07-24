/*****************************************************************
 * $Id: acc_actioncostcenterbudget.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jun 21, 2012 14:00:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actioncostcenterbudget.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionCostCenterBudget::ACC_ActionCostCenterBudget()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionCostCenterBudget::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionCostCenterBudget::getName(),
                              //QIcon(":/images/icons/star.png"),
                              tr("C&enter and Budget"));
        ga->setStatusTip(tr("Cost center and budget maintenance"));
        ga->setFactory(ACC_ActionCostCenterBudget::factory);
    }
    return ga;
}

RB_Action* ACC_ActionCostCenterBudget::factory() {
    RB_Action* a = new ACC_ActionCostCenterBudget();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionCostCenterBudget::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogCostCenterBudget);
    dlg->exec();
    dlg->deleteLater();
}
