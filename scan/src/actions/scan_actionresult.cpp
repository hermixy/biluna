/*****************************************************************
 * $Id: scan_actionresult.cpp 1471 2011-10-20 18:42:46Z rutger $
 * Created: Apr 28, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_actionresult.h"

#include "scan_dialogfactory.h"
#include "scan_modelfactory.h"
#include "rb_mdiwindow.h"


SCAN_ActionResult::SCAN_ActionResult()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SCAN_ActionResult::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SCAN_ActionResult::getName(),
                              QIcon(":/images/icons/report_user.png"),
                              tr("Scan result"));
        ga->setStatusTip(tr("Scan result"));
        ga->setFactory(SCAN_ActionResult::factory);
    }
    return ga;
}

RB_Action* SCAN_ActionResult::factory() {
    RB_Action* a = new SCAN_ActionResult();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SCAN_ActionResult::trigger() {
    // Check required settings
    if (SCAN_MODELFACTORY->getRootId() == ""
            || !SCAN_MODELFACTORY->getDatabase().isOpen()) {
        SCAN_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and than select project."));
        return;
    }

    // Create widget
    RB_MdiWindow* mdiWin = SCAN_DIALOGFACTORY->getMdiWindow(
            SCAN_DialogFactory::WidgetScanResult);
    mdiWin->show();
    mdiWin->raise();
}
