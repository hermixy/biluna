/*****************************************************************
 * $Id: peng_actionfluidstability.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: July 29, 2014 20:14:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_actionfluidstability.h"

#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "rb_dialog.h"


PENG_ActionFluidStability::PENG_ActionFluidStability()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PENG_ActionFluidStability::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PENG_ActionFluidStability::getName(),
                              // QIcon(":/images/icons/equipmenttype_edit.png"),
                              tr("Fluid &Stability"));
        ga->setStatusTip(tr("Edit fluid stability"));
        ga->setFactory(PENG_ActionFluidStability::factory);
    }
    return ga;
}

RB_Action* PENG_ActionFluidStability::factory() {
    RB_Action* a = new PENG_ActionFluidStability();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void PENG_ActionFluidStability::trigger() {
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
    RB_Dialog* dlg = PENG_DIALOGFACTORY->getDialog(PENG_DialogFactory::DialogFluidStability);
    dlg->exec();
    dlg->deleteLater();
}
