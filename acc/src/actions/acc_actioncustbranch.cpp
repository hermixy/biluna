/*****************************************************************
 * $Id: acc_actioncustbranch.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Dec 24, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actioncustbranch.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionCustBranch::ACC_ActionCustBranch()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionCustBranch::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionCustBranch::getName(),
                              QIcon(":/images/icons/group_edit.png"),
                              tr("Customer &Branch"));
        ga->setStatusTip(tr("Customer Branch"));
        ga->setFactory(ACC_ActionCustBranch::factory);
    }
    return ga;
}

RB_Action* ACC_ActionCustBranch::factory() {
    RB_Action* a = new ACC_ActionCustBranch();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionCustBranch::trigger() {
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
            ACC_DialogFactory::WidgetCustBranch);

    mdiWin->show();
    mdiWin->raise();
}
