/*****************************************************************
 * $Id: peng_actioneditproject.cpp 2210 2015-01-22 14:59:25Z rutger $
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
#include "rb_dialogwindow.h"


PENG_ActionEditProject::PENG_ActionEditProject()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PENG_ActionEditProject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PENG_ActionEditProject::getName(),
                                  "dialog",
                                  tr("&Project"));
        ga->setStatusTip(tr("Edit project"));
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
    RB_DialogWindow* dlg = PENG_DIALOGFACTORY->getDialogWindow(
                PENG_DialogFactory::WidgetProjectEdit);
    dlg->exec();
    dlg->deleteLater();
}
