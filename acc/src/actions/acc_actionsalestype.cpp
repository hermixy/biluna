/*****************************************************************
 * $Id: acc_actionsalestype.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 20, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionsalestype.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionSalesType::ACC_ActionSalesType()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionSalesType::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionSalesType::getName(),
                              QIcon(":/images/icons/money_list.png"),
                              tr("&Sales Types/Price Lists"));
        ga->setStatusTip(tr("Sales types or price lists"));
        ga->setFactory(ACC_ActionSalesType::factory);
    }
    return ga;
}

RB_Action* ACC_ActionSalesType::factory() {
    RB_Action* a = new ACC_ActionSalesType();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionSalesType::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogSalesType);
    dlg->exec();
    dlg->deleteLater();
}
