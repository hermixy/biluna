/*****************************************************************
 * $Id: acc_actiontrialbalance.cpp 2131 2014-03-28 19:46:53Z rutger $
 * Created: Oct 13, 2010 10:59:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actiontrialbalance.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionTrialBalance::ACC_ActionTrialBalance()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionTrialBalance::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionTrialBalance::getName(),
                              QIcon(":/images/icons/report_coins.png"),
                              tr("&Trial balance"));
        ga->setStatusTip(tr("View trial balance"));
        ga->setFactory(ACC_ActionTrialBalance::factory);
    }
    return ga;
}

RB_Action* ACC_ActionTrialBalance::factory() {
    RB_Action* a = new ACC_ActionTrialBalance();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionTrialBalance::trigger() {
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
            ACC_DialogFactory::WidgetTrialBalance);
    mdiWin->show();
    mdiWin->raise();

    // show toolbars
    QToolBar* toolBar = ACC_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_edit");
    if (toolBar) {
        toolBar->show();
    }
}
