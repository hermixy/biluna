/*****************************************************************
 * $Id: acc_actionsupplier.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Jan 17, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionsupplier.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionSupplier::ACC_ActionSupplier()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionSupplier::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionSupplier::getName(),
                              QIcon(":/images/icons/group_gear.png"),
                              tr("Supplier &Branch"));
        ga->setStatusTip(tr("Supplier Branch"));
        ga->setFactory(ACC_ActionSupplier::factory);
    }
    return ga;
}

RB_Action* ACC_ActionSupplier::factory() {
    RB_Action* a = new ACC_ActionSupplier();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionSupplier::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create widget, supplier master is created by dialogfactory
    RB_MdiWindow* mdiWin = ACC_DIALOGFACTORY->getMdiWindow(
            ACC_DialogFactory::WidgetSupplierDetail);
    mdiWin->show();
    mdiWin->raise();
}
