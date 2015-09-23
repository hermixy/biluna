/*****************************************************************
 * $Id: acc_actionbankimport.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Oct 15, 2012 18:00:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionbankimport.h"

#include "acc_actiongltransaction.h"
#include "acc_dialogfactory.h"
#include "acc_gltransactionwidget.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionBankImport::ACC_ActionBankImport()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionBankImport::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionBankImport::getName(),
                              QIcon(":/images/icons/vault_go.png"),
                              tr("Import &bank"));
        ga->setStatusTip(tr("Import bank statements"));
        ga->setFactory(ACC_ActionBankImport::factory);
    }
    return ga;
}

RB_Action* ACC_ActionBankImport::factory() {
    RB_Action* a = new ACC_ActionBankImport();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionBankImport::trigger() {
    ACC_ActionGlTransaction action;
    action.trigger();

//    // Check required settings
//    if (ACC_MODELFACTORY->getRootId() == ""
//            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
//        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
//                                                   "Connect first to database\n"
//                                                   "and then select project."));
//        return;
//    }

    // Check widget state
    RB_MdiWindow* mdiWin = ACC_DIALOGFACTORY->getMdiWindow(
            ACC_DialogFactory::WidgetGlTransaction);

    if (mdiWin->isWindowModified()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Data in GL Transaction window is modified.\n"
                                                   "Please save your data first."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogBankImport);
    dlg->exec();
    dlg->deleteLater();

    // TODO: set corresponding bank account in ACC_GlTransactionWidget
    // ACC_GlTransactionWidget* wdgt = dynamic_cast<ACC_GlTransactionWidget*>(mdiWin);
    // ...
}
