/*****************************************************************
 * $Id: acc_actiontaxauthrate.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Feb 22, 2010 1:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actiontaxauthrate.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionTaxAuthRate::ACC_ActionTaxAuthRate()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionTaxAuthRate::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionTaxAuthRate::getName(),
                              //QIcon(":/images/icons/star.png"),
                              tr("Tax Authorities/&Rates"));
        ga->setStatusTip(tr("Tax authorities and rates maintenance"));
        ga->setFactory(ACC_ActionTaxAuthRate::factory);
    }
    return ga;
}

RB_Action* ACC_ActionTaxAuthRate::factory() {
    RB_Action* a = new ACC_ActionTaxAuthRate();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionTaxAuthRate::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogTaxAuthRate);
    dlg->exec();
    dlg->deleteLater();
}
