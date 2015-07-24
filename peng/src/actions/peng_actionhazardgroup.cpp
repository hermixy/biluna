/*****************************************************************
 * $Id: peng_actionfluid.cpp 2146 2014-05-23 20:27:06Z rutger $
 * Created: June 16, 2014 4:14:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_actionhazardgroup.h"

#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "rb_dialog.h"


PENG_ActionHazardGroup::PENG_ActionHazardGroup()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PENG_ActionHazardGroup::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PENG_ActionHazardGroup::getName(),
                              // QIcon(":/images/icons/equipmenttype_edit.png"),
                              tr("&Hazard group"));
        ga->setStatusTip(tr("Edit hazard group"));
        ga->setFactory(PENG_ActionHazardGroup::factory);
    }
    return ga;
}

RB_Action* PENG_ActionHazardGroup::factory() {
    RB_Action* a = new PENG_ActionHazardGroup();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void PENG_ActionHazardGroup::trigger() {
    // Check required settings
    if (PENG_MODELFACTORY->getRootId() == ""
            || !PENG_MODELFACTORY->getDatabase().isOpen()) {
        PENG_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create widget
    // Create dialog
    RB_Dialog* dlg = PENG_DIALOGFACTORY->getDialog(PENG_DialogFactory::DialogHazardGroup);
    dlg->exec();
    dlg->deleteLater();
}
