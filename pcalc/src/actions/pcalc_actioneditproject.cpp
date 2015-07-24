/*****************************************************************
 * $Id: peng_actioneditproject.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Apr 29, 2015 4:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_actioneditproject.h"

#include "pcalc_dialogfactory.h"
#include "pcalc_modelfactory.h"
#include "rb_database.h"
#include "rb_mainwindow.h"


PCALC_ActionEditProject::PCALC_ActionEditProject()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PCALC_ActionEditProject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PCALC_ActionEditProject::getName(),
                                  "dialog",
                                  tr("&Project"));
        ga->setStatusTip(tr("Edit project"));
        ga->setFactory(PCALC_ActionEditProject::factory);
    }
    return ga;
}

RB_Action* PCALC_ActionEditProject::factory() {
    RB_Action* a = new PCALC_ActionEditProject();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void PCALC_ActionEditProject::trigger() {
    // Check required settings
    if (PCALC_MODELFACTORY->getRootId() == ""
            || !PCALC_MODELFACTORY->getDatabase().isOpen()) {
        PCALC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = PCALC_DIALOGFACTORY->getDialog(PCALC_DialogFactory::DialogProjectEdit);
    dlg->exec();
    dlg->deleteLater();
}
