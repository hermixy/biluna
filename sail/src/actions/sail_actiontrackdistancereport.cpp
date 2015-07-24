/*****************************************************************
 * $Id: sail_actiontrackdistancereport.cpp 1736 2012-08-16 14:37:58Z rutger $
 * Created: Aug 11, 2012 8:58:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_actiontrackdistancereport.h"

#include "sail_dialogfactory.h"
#include "sail_modelfactory.h"
#include "rb_mdiwindow.h"


SAIL_ActionTrackDistanceReport::SAIL_ActionTrackDistanceReport()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SAIL_ActionTrackDistanceReport::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SAIL_ActionTrackDistanceReport::getName(),
                              QIcon(":/images/icons/sail_report.png"),
                              tr("Trac&k Distance Report"));
        ga->setStatusTip(tr("Map navigation"));
        ga->setFactory(SAIL_ActionTrackDistanceReport::factory);
    }
    return ga;
}

RB_Action* SAIL_ActionTrackDistanceReport::factory() {
    RB_Action* a = new SAIL_ActionTrackDistanceReport();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SAIL_ActionTrackDistanceReport::trigger() {
    // Check required settings
    if (SAIL_MODELFACTORY->getRootId() == ""
            || !SAIL_MODELFACTORY->getDatabase().isOpen()) {
        SAIL_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and than select project."));
        return;
    }

    // Create widget
    RB_MdiWindow* mdiWin = SAIL_DIALOGFACTORY->getMdiWindow(
            SAIL_DialogFactory::WidgetTrackDistanceReport);

    mdiWin->show();
    mdiWin->raise();
}
