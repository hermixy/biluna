/*****************************************************************
 * $Id: acc_actioncontactsupplieredit.cpp 2153 2014-07-25 16:38:28Z rutger $
 * Created: Jan 06, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actioncontactsupplieredit.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionContactSupplierEdit::ACC_ActionContactSupplierEdit()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionContactSupplierEdit::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionContactSupplierEdit::getName(),
                              QIcon(":/images/icons/group_link_supplier.png"),
                              tr("Supplier &Contacts"));
        ga->setStatusTip(tr("Edit Supplier Contacts"));
        ga->setFactory(ACC_ActionContactSupplierEdit::factory);
    }
    return ga;
}

RB_Action* ACC_ActionContactSupplierEdit::factory() {
    RB_Action* a = new ACC_ActionContactSupplierEdit();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionContactSupplierEdit::trigger() {
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
            ACC_DialogFactory::WidgetContactSupplierEdit);
    mdiWin->show();
    mdiWin->raise();
}
