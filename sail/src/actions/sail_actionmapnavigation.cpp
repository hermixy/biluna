/*****************************************************************
 * $Id: sail_actionmapnavigation.cpp 1736 2012-08-16 14:37:58Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_actionmapnavigation.h"

#include "sail_dialogfactory.h"
#include "sail_modelfactory.h"
#include "rb_mdiwindow.h"


SAIL_ActionMapNavigation::SAIL_ActionMapNavigation()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SAIL_ActionMapNavigation::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SAIL_ActionMapNavigation::getName(),
                              QIcon(":/images/icons/sail_world.png"),
                              tr("&Map Navigation"));
        ga->setStatusTip(tr("Map navigation"));
        ga->setFactory(SAIL_ActionMapNavigation::factory);
    }
    return ga;
}

RB_Action* SAIL_ActionMapNavigation::factory() {
    RB_Action* a = new SAIL_ActionMapNavigation();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SAIL_ActionMapNavigation::trigger() {
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
            SAIL_DialogFactory::WidgetMapNavigation);

    mdiWin->show();
    mdiWin->raise();
}
