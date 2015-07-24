/*****************************************************************
 * $Id: sail_actionclose.cpp 2185 2014-10-09 15:09:35Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "mrp_actionclose.h"

#include "db_actionfactory.h"


MRP_ActionClose::MRP_ActionClose()
        : RB_Action() {
    // nothing
}


RB_GuiAction* MRP_ActionClose::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(MRP_ActionClose::getName(),
                              QIcon(":/images/icons/cross.png"),
                              tr("Close"));
        ga->setStatusTip(tr("Close this perspective"));
        ga->setFactory(MRP_ActionClose::factory);
    }
    return ga;
}

RB_Action* MRP_ActionClose::factory() {
    RB_Action* a = new MRP_ActionClose();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void MRP_ActionClose::trigger() {
    DB_ACTIONFACTORY->closePlugin("MRP");
}
