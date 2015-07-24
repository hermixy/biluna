/*****************************************************************
 * $Id: acc_actioncustcostpart.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Sept 25, 2012 12:20:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actioncustcostpart.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionCustCostPart::ACC_ActionCustCostPart()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionCustCostPart::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionCustCostPart::getName(),
//                              QIcon(":/images/icons/group_edit.png"),
                              tr("&Customer Part"));
        ga->setStatusTip(tr("Customer Cost Center Part"));
        ga->setFactory(ACC_ActionCustCostPart::factory);
    }
    return ga;
}

RB_Action* ACC_ActionCustCostPart::factory() {
    RB_Action* a = new ACC_ActionCustCostPart();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionCustCostPart::trigger() {
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
            ACC_DialogFactory::WidgetCustCostPart);

    mdiWin->show();
    mdiWin->raise();
}
