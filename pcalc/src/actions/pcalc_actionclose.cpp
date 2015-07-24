/*****************************************************************
 * $Id: peng_actionclose.cpp 2185 2014-10-09 15:09:35Z rutger $
 * Created: Apr 29, 2015 4:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_actionclose.h"

#include "db_actionfactory.h"


PCALC_ActionClose::PCALC_ActionClose()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PCALC_ActionClose::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PCALC_ActionClose::getName(),
                              QIcon(":/images/icons/cross.png"),
                              tr("Close"));
        ga->setStatusTip(tr("Close this perspective"));
        ga->setFactory(PCALC_ActionClose::factory);
    }
    return ga;
}

RB_Action* PCALC_ActionClose::factory() {
    RB_Action* a = new PCALC_ActionClose();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void PCALC_ActionClose::trigger() {
    DB_ACTIONFACTORY->closePlugin("PCALC");
}
