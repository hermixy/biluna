/*****************************************************************
 * $Id: peng_actionclose.cpp 2185 2014-10-09 15:09:35Z rutger $
 * Created: Oct 8, 2014 8:14:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_actionclose.h"

#include "db_actionfactory.h"


PENG_ActionClose::PENG_ActionClose()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PENG_ActionClose::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PENG_ActionClose::getName(),
                              QIcon(":/images/icons/cross.png"),
                              tr("Close"));
        ga->setStatusTip(tr("Close this perspective"));
        ga->setFactory(PENG_ActionClose::factory);
    }
    return ga;
}

RB_Action* PENG_ActionClose::factory() {
    RB_Action* a = new PENG_ActionClose();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void PENG_ActionClose::trigger() {
    DB_ACTIONFACTORY->closePlugin("PENG");
}
