/*****************************************************************
 * $Id: acc_actionsupplcostpart.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Sept 25, 2012 12:20:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionsupplcostpart.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionSupplCostPart::ACC_ActionSupplCostPart()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionSupplCostPart::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionSupplCostPart::getName(),
//                              QIcon(":/images/icons/group_edit.png"),
                              tr("&Supplier Part"));
        ga->setStatusTip(tr("Supplier Cost Center Part"));
        ga->setFactory(ACC_ActionSupplCostPart::factory);
    }
    return ga;
}

RB_Action* ACC_ActionSupplCostPart::factory() {
    RB_Action* a = new ACC_ActionSupplCostPart();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionSupplCostPart::trigger() {
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
            ACC_DialogFactory::WidgetSupplCostPart);

    mdiWin->show();
    mdiWin->raise();
}
