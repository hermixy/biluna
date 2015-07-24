/*****************************************************************
 * $Id: sail_actiontrackdistancereport.cpp 1736 2012-08-16 14:37:58Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "mrp_actionsoftwarelicensereport.h"

#include "mrp_dialogfactory.h"
#include "mrp_modelfactory.h"
#include "rb_mdiwindow.h"


MRP_ActionSoftwareLicenseReport::MRP_ActionSoftwareLicenseReport()
        : RB_Action() {
    // nothing
}


RB_GuiAction* MRP_ActionSoftwareLicenseReport::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(MRP_ActionSoftwareLicenseReport::getName(),
                              QIcon(":/images/icons/report_key.png"),
                              tr("Softw&are License Report"));
        ga->setStatusTip(tr("Software license report"));
        ga->setFactory(MRP_ActionSoftwareLicenseReport::factory);
    }
    return ga;
}

RB_Action* MRP_ActionSoftwareLicenseReport::factory() {
    RB_Action* a = new MRP_ActionSoftwareLicenseReport();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void MRP_ActionSoftwareLicenseReport::trigger() {
    // Check required settings
    if (MRP_MODELFACTORY->getRootId() == ""
            || !MRP_MODELFACTORY->getDatabase().isOpen()) {
        MRP_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and than select project."));
        return;
    }

    // Create widget
    RB_MdiWindow* mdiWin = MRP_DIALOGFACTORY->getMdiWindow(
            MRP_DialogFactory::WidgetSoftwareLicenseReport);

    mdiWin->show();
    mdiWin->raise();
}
