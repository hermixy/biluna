/*****************************************************************
 * $Id: acc_actioncurrency.cpp 1101 2010-02-18 22:40:13Z rutger $
 * Created: Aug 29, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actiongltransaction.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionGlTransaction::ACC_ActionGlTransaction()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionGlTransaction::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionGlTransaction::getName(),
                              QIcon(":/images/icons/money_accounting.png"),
                              tr("G&L Transactions/Accounting"));
        ga->setStatusTip(tr("General Ledger Transactions"));
        ga->setFactory(ACC_ActionGlTransaction::factory);
    }
    return ga;
}

RB_Action* ACC_ActionGlTransaction::factory() {
    RB_Action* a = new ACC_ActionGlTransaction();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionGlTransaction::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    // Create widget and corresponding dockwidget if applicable
    RB_MdiWindow* mdiWin = ACC_DIALOGFACTORY->getMdiWindow(
            ACC_DialogFactory::WidgetGlTransaction);
    mdiWin->show();
    mdiWin->raise();
    QApplication::restoreOverrideCursor();
}
