/*****************************************************************
 * $Id: scan_actioneditproject.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Dec 14, 2012 9:57:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_actioneditproject.h"

#include "scan_dialogfactory.h"
#include "scan_modelfactory.h"
#include "rb_database.h"
//#include "rb_mainwindow.h"


SCAN_ActionEditProject::SCAN_ActionEditProject()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SCAN_ActionEditProject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SCAN_ActionEditProject::getName(),
                                  "dialog",
                                  tr("&Edit project"));
        ga->setFactory(SCAN_ActionEditProject::factory);
    }
    return ga;
}

RB_Action* SCAN_ActionEditProject::factory() {
    RB_Action* a = new SCAN_ActionEditProject();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SCAN_ActionEditProject::trigger() {
    // Check required settings
    if (SCAN_MODELFACTORY->getRootId() == ""
            || !SCAN_MODELFACTORY->getDatabase().isOpen()) {
        SCAN_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = SCAN_DIALOGFACTORY->getDialog(SCAN_DialogFactory::DialogProjectEdit);
    dlg->exec();
    dlg->deleteLater();
}
