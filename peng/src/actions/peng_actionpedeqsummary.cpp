/*****************************************************************
 * $Id: peng_actionpedeqsummary.cpp 1399 2011-04-16 08:11:44Z rutger $
 * Created: Mar 02, 2011 10:59:42 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_actionpedeqsummary.h"

#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "rb_mdiwindow.h"


PENG_ActionPedEqSummary::PENG_ActionPedEqSummary()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PENG_ActionPedEqSummary::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PENG_ActionPedEqSummary::getName(),
                              QIcon(":/images/icons/report_go.png"),
                              tr("&PED Summary"));
        ga->setStatusTip(tr("View PED summary"));
        ga->setFactory(PENG_ActionPedEqSummary::factory);
    }
    return ga;
}

RB_Action* PENG_ActionPedEqSummary::factory() {
    RB_Action* a = new PENG_ActionPedEqSummary();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void PENG_ActionPedEqSummary::trigger() {
    // Check required settings
    if (PENG_MODELFACTORY->getRootId() == ""
            || !PENG_MODELFACTORY->getDatabase().isOpen()) {
        PENG_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create widget and corresponding dockwidget if applicable
    RB_MdiWindow* mdiWin = PENG_DIALOGFACTORY->getMdiWindow(
            PENG_DialogFactory::WidgetPedEqSummary);
    mdiWin->show();
    PENG_DIALOGFACTORY->getMainWindow()->setActiveMdiWindow(mdiWin);
}
