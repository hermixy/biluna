/*****************************************************************
 * $Id: sail_actiontrackdistancereport.cpp 1736 2012-08-16 14:37:58Z rutger $
 * Created: Oct 14, 2015 21:17:25 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionbankpaymentreport.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionBankPaymentReport::ACC_ActionBankPaymentReport()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionBankPaymentReport::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionBankPaymentReport::getName(),
//                              QIcon(":/images/icons/report_key.png"),
                              tr("Ba&nk Payment Report"));
        ga->setStatusTip(tr("Software license report"));
        ga->setFactory(ACC_ActionBankPaymentReport::factory);
    }
    return ga;
}

RB_Action* ACC_ActionBankPaymentReport::factory() {
    RB_Action* a = new ACC_ActionBankPaymentReport();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionBankPaymentReport::trigger() {
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
            ACC_DialogFactory::WidgetBankPaymentReport);

    mdiWin->show();
    mdiWin->raise();
}
