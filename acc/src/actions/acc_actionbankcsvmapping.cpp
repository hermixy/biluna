/*****************************************************************
 * $Id: acc_actionbankcsvmapping.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jul 01, 2013 10:04:25 AM - rutger
 *
 * Copyright (C) 2013 Biluna. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionbankcsvmapping.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionBankCsvMapping::ACC_ActionBankCsvMapping()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionBankCsvMapping::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionBankCsvMapping::getName(),
//                              QIcon(":/images/icons/vault_go.png"),
                              tr("Bank CS&V Import Mapping"));
        ga->setStatusTip(tr("Bank CSV import mapping"));
        ga->setFactory(ACC_ActionBankCsvMapping::factory);
    }
    return ga;
}

RB_Action* ACC_ActionBankCsvMapping::factory() {
    RB_Action* a = new ACC_ActionBankCsvMapping();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionBankCsvMapping::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogBankCsvMapping);
    dlg->exec();
    dlg->deleteLater();

}
