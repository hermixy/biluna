/*****************************************************************
 * $Id: acc_actionpurchaseorder.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: May 26, 2010 1:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionpurchaseorder.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionPurchaseOrder::ACC_ActionPurchaseOrder()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionPurchaseOrder::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionPurchaseOrder::getName(),
                              QIcon(":/images/icons/bricks_back.png"),
                              tr("&Purchase Orders"));
        ga->setStatusTip(tr("Purchase Orders"));
        ga->setFactory(ACC_ActionPurchaseOrder::factory);
    }
    return ga;
}

RB_Action* ACC_ActionPurchaseOrder::factory() {
    RB_Action* a = new ACC_ActionPurchaseOrder();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionPurchaseOrder::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create widget and corresponding dockwidget if applicable
    RB_MdiWindow* mdiWin = ACC_DIALOGFACTORY->getMdiWindow(
            ACC_DialogFactory::WidgetPurchaseOrder);

    mdiWin->show();
    mdiWin->raise();
}
