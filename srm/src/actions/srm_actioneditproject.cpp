/*****************************************************************
 * $Id: srm_actioneditproject.cpp 1893 2013-02-21 21:41:33Z rutger $
 * Created: Dec 14, 2012 9:57:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_actioneditproject.h"

#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "rb_database.h"
#include "rb_mainwindow.h"


PENG_ActionEditProject::PENG_ActionEditProject()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PENG_ActionEditProject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PENG_ActionEditProject::getName(),
                                  "dialog",
                                  tr("&Edit project"));
        ga->setFactory(PENG_ActionEditProject::factory);
    }
    return ga;
}

RB_Action* PENG_ActionEditProject::factory() {
    RB_Action* a = new PENG_ActionEditProject();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void PENG_ActionEditProject::trigger() {
    // Check required settings
    if (PENG_MODELFACTORY->getRootId() == ""
            || !PENG_MODELFACTORY->getDatabase().isOpen()) {
        PENG_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = PENG_DIALOGFACTORY->getDialog(PENG_DialogFactory::DialogProjectEdit);
    dlg->exec();
    delete dlg;
}
