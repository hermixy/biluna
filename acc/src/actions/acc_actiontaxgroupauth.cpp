/*****************************************************************
 * $Id: acc_actiontaxgroupauth.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 20, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actiontaxgroupauth.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionTaxGroupAuth::ACC_ActionTaxGroupAuth()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionTaxGroupAuth::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionTaxGroupAuth::getName(),
                              //QIcon(":/images/icons/star.png"),
                              tr("Tax &Group/Authorities"));
        ga->setStatusTip(tr("Tax group and authority maintenance, "
                            "for customer and supplier"));
        ga->setFactory(ACC_ActionTaxGroupAuth::factory);
    }
    return ga;
}

RB_Action* ACC_ActionTaxGroupAuth::factory() {
    RB_Action* a = new ACC_ActionTaxGroupAuth();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionTaxGroupAuth::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogTaxGroupAuth);
    dlg->exec();
    dlg->deleteLater();
}
