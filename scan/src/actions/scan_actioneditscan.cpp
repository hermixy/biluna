/*****************************************************************
 * $Id: scan_actioneditscan.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Apr 26, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_actioneditscan.h"

#include "scan_dialogfactory.h"
#include "scan_modelfactory.h"
#include "rb_dialog.h"


SCAN_ActionEditScan::SCAN_ActionEditScan()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SCAN_ActionEditScan::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SCAN_ActionEditScan::getName(),
                              QIcon(":/images/icons/plugin_edit.png"),
                              tr("Edit scan"));
        ga->setStatusTip(tr("Edit scan"));
        ga->setFactory(SCAN_ActionEditScan::factory);
    }
    return ga;
}

RB_Action* SCAN_ActionEditScan::factory() {
    RB_Action* a = new SCAN_ActionEditScan();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SCAN_ActionEditScan::trigger() {
    // Check required settings
    if (SCAN_MODELFACTORY->getRootId() == ""
            || !SCAN_MODELFACTORY->getDatabase().isOpen()) {
        SCAN_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and than select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = SCAN_DIALOGFACTORY->getDialog(SCAN_DialogFactory::DialogScanEdit);
    dlg->exec();
    dlg->deleteLater();
}
