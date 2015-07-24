/*****************************************************************
 * $Id: acc_actioninvoiceaccruedreport.cpp 2202 2015-01-11 19:39:36Z rutger $
 * Created: Jan 11, 2015 - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actioninvoiceaccruedreport.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionInvoiceAccruedReport::ACC_ActionInvoiceAccruedReport()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionInvoiceAccruedReport::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionInvoiceAccruedReport::getName(),
                              // QIcon(":/images/icons/report_key.png"),
                              tr("Invoice Accr&ued Report"));
        ga->setStatusTip(tr("Invoice accrued report"));
        ga->setFactory(ACC_ActionInvoiceAccruedReport::factory);
    }
    return ga;
}

RB_Action* ACC_ActionInvoiceAccruedReport::factory() {
    RB_Action* a = new ACC_ActionInvoiceAccruedReport();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionInvoiceAccruedReport::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and than select project."));
        return;
    }

    // Create widget
    RB_MdiWindow* mdiWin = ACC_DIALOGFACTORY->getMdiWindow(
            ACC_DialogFactory::WidgetInvoiceAccruedReport);

    mdiWin->show();
    mdiWin->raise();
}
