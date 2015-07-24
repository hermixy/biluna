/*****************************************************************
 * $Id: sail_actionclose.cpp 2185 2014-10-09 15:09:35Z rutger $
 * Created: Oct 8, 2014 8:14:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_actionclose.h"

#include "db_actionfactory.h"


SAIL_ActionClose::SAIL_ActionClose()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SAIL_ActionClose::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SAIL_ActionClose::getName(),
                              QIcon(":/images/icons/cross.png"),
                              tr("Close"));
        ga->setStatusTip(tr("Close this perspective"));
        ga->setFactory(SAIL_ActionClose::factory);
    }
    return ga;
}

RB_Action* SAIL_ActionClose::factory() {
    RB_Action* a = new SAIL_ActionClose();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SAIL_ActionClose::trigger() {
    DB_ACTIONFACTORY->closePlugin("SAIL");
}
