/*****************************************************************
 * $Id: acc_actionprice.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Jan 20, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionprice.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionPrice::ACC_ActionPrice()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionPrice::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionPrice::getName(),
                              QIcon(":/images/icons/money.png"),
                              tr("&Price List"));
        ga->setStatusTip(tr("Item price"));
        ga->setFactory(ACC_ActionPrice::factory);
    }
    return ga;
}

RB_Action* ACC_ActionPrice::factory() {
    RB_Action* a = new ACC_ActionPrice();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionPrice::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create widget
    RB_MdiWindow* mdiWin = ACC_DIALOGFACTORY->getMdiWindow(
            ACC_DialogFactory::WidgetPrice);
    mdiWin->show();
    mdiWin->raise();
}
