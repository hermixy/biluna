/*****************************************************************
 * $Id: peng_actionequipment.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Feb 16, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_actionequipment.h"

#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "rb_mdiwindow.h"


PENG_ActionEquipment::PENG_ActionEquipment()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PENG_ActionEquipment::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PENG_ActionEquipment::getName(),
                              QIcon(":/images/icons/equipment.png"),
                              tr("E&quipment"));
        ga->setStatusTip(tr("Equipment list detail"));
        ga->setFactory(PENG_ActionEquipment::factory);
    }
    return ga;
}

RB_Action* PENG_ActionEquipment::factory() {
    RB_Action* a = new PENG_ActionEquipment();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void PENG_ActionEquipment::trigger() {
    // Check required settings
    if (PENG_MODELFACTORY->getRootId() == ""
            || !PENG_MODELFACTORY->getDatabase().isOpen()) {
        PENG_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

}
