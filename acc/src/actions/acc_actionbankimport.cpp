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
    try {
        ACC_ActionGlTransaction action;
        action.trigger();

        // Check widget state.
        // TODO: crashes application?
        RB_MdiWindow* mdiWin = ACC_DIALOGFACTORY->getMdiWindow(
                ACC_DialogFactory::WidgetGlTransaction);

        if (mdiWin->isWindowModified()) {
            ACC_DIALOGFACTORY->requestWarningDialog(
                        tr("Data in GL Transaction window is modified.\n"
                           "Please save your data first."));
            return;
        }

        // Create dialog
        RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(
                    ACC_DialogFactory::DialogBankImport);
        if (dlg->exec() == QDialog::Accepted) {
            // TODO: set corresponding bank account in ACC_GlTransactionWidget
            ACC_GlTransactionWidget* wdgt =
                    dynamic_cast<ACC_GlTransactionWidget*>(mdiWin->getWidget());
            wdgt->slotRefreshBankModel();
        }

        dlg->deleteLater();
    } catch(std::exception& e) {
        ACC_DIALOGFACTORY->requestWarningDialog(
                    "Error " + QString(e.what())
                    + "\nin ACC_ActionBankImport::trigger()");
    } catch(...) {
        ACC_DIALOGFACTORY->requestWarningDialog(
                    "Error <unkown> in \n"
                    "ACC_ActionBankImport::trigger()");
    }
}
