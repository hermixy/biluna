/*****************************************************************
 * $Id: sail_actionmapnavigation.cpp 1736 2012-08-16 14:37:58Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "mrp_actionsoftwarelicense.h"

#include "mrp_dialogfactory.h"
#include "mrp_modelfactory.h"
#include "rb_mdiwindow.h"


MRP_ActionSoftwareLicense::MRP_ActionSoftwareLicense()
        : RB_Action() {
    // nothing
}


RB_GuiAction* MRP_ActionSoftwareLicense::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(MRP_ActionSoftwareLicense::getName(),
                              QIcon(":/images/icons/application_key.png"),
                              tr("&Software License"));
        ga->setStatusTip(tr("Software license"));
        ga->setFactory(MRP_ActionSoftwareLicense::factory);
    }
    return ga;
}

RB_Action* MRP_ActionSoftwareLicense::factory() {
    RB_Action* a = new MRP_ActionSoftwareLicense();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void MRP_ActionSoftwareLicense::trigger() {
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
            MRP_DialogFactory::WidgetSoftwareLicense);

    mdiWin->show();
    mdiWin->raise();
}
