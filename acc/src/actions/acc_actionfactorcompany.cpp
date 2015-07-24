/*****************************************************************
 * $Id: acc_actionfactorcompany.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 20, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionfactorcompany.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionFactorCompany::ACC_ActionFactorCompany()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionFactorCompany::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionFactorCompany::getName(),
                              //QIcon(":/images/icons/star.png"),
                              tr("&Factor Companies"));
        ga->setStatusTip(tr("Factor companies maintenance"));
        ga->setFactory(ACC_ActionFactorCompany::factory);
    }
    return ga;
}

RB_Action* ACC_ActionFactorCompany::factory() {
    RB_Action* a = new ACC_ActionFactorCompany();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionFactorCompany::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogFactorCompany);
    dlg->exec();
    dlg->deleteLater();
}
