/*****************************************************************
 * $Id: acc_actionrecreatebalance.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Sept 19, 2011 14:14:42 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionrecreatebalance.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_postgltransaction.h"
#include "acc_selectyeardialog.h"


ACC_ActionRecreateBalance::ACC_ActionRecreateBalance()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionRecreateBalance::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionRecreateBalance::getName(),
                              QIcon(":/images/icons/database_gear.png"),
                              tr("&Recreate Balance"));
        ga->setStatusTip(tr("Recreate balance summaries"));
        ga->setFactory(ACC_ActionRecreateBalance::factory);
    }
    return ga;
}

RB_Action* ACC_ActionRecreateBalance::factory() {
    RB_Action* a = new ACC_ActionRecreateBalance();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action
 * TODO: implement period (months only) selection dialog
 */
void ACC_ActionRecreateBalance::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    int ret = ACC_DIALOGFACTORY->requestYesNoDialog(
                tr("Recreate Balance Summaries"),
                tr("Balance will be recreated.\n"
                   "This will take some time.\n"
                   "Do you want to continue?"));
    if (ret == QMessageBox::No) {
        ACC_DIALOGFACTORY->statusBarMessage(tr("No action"), 4000);
        return;
    }

    ACC_SelectYearDialog* dlg = new ACC_SelectYearDialog(ACC_DIALOGFACTORY->getMainWindow());
    dlg->init();

    if (QDialog::Accepted != dlg->exec()) {
        dlg->deleteLater();
        return;
    }

    int year = dlg->getYear();
    dlg->deleteLater();

    // Start end period, TODO: implement period (months only) selection dialog
    int start = year * 100 + 1;
    int end = start + 11;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    ACC_PostGlTransaction oper;
    oper.recreate(start, end);
    QApplication::restoreOverrideCursor();
    ACC_DIALOGFACTORY->statusBarMessage(tr("Balance summary recreated."), 4000);
}
