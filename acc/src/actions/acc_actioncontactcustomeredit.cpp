/*****************************************************************
 * $Id: acc_actioncontactcustomeredit.cpp 2153 2014-07-25 16:38:28Z rutger $
 * Created: Jan 06, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actioncontactcustomeredit.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionContactCustomerEdit::ACC_ActionContactCustomerEdit()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionContactCustomerEdit::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionContactCustomerEdit::getName(),
                              QIcon(":/images/icons/group_link.png"),
                              tr("Customer &Contacts"));
        ga->setStatusTip(tr("Edit Customer Contacts"));
        ga->setFactory(ACC_ActionContactCustomerEdit::factory);
    }
    return ga;
}

RB_Action* ACC_ActionContactCustomerEdit::factory() {
    RB_Action* a = new ACC_ActionContactCustomerEdit();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionContactCustomerEdit::trigger() {
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
            ACC_DialogFactory::WidgetContactCustomerEdit);
    mdiWin->show();
    mdiWin->raise();
}
