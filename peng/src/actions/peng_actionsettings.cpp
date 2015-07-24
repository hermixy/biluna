/*****************************************************************
 * $Id: peng_actionsettings.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Aug 4, 2014 12:57:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_actionsettings.h"

#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "rb_database.h"
#include "rb_mainwindow.h"


PENG_ActionSettings::PENG_ActionSettings()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PENG_ActionSettings::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PENG_ActionSettings::getName(),
                                  "dialog",
                                  tr("&Settings"));
        ga->setStatusTip(tr("Edit main Settings"));
        ga->setFactory(PENG_ActionSettings::factory);
    }
    return ga;
}

RB_Action* PENG_ActionSettings::factory() {
    RB_Action* a = new PENG_ActionSettings();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void PENG_ActionSettings::trigger() {
    // Check required settings
    if (PENG_MODELFACTORY->getRootId() == ""
            || !PENG_MODELFACTORY->getDatabase().isOpen()) {
        PENG_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = PENG_DIALOGFACTORY->getDialog(PENG_DialogFactory::DialogSettings);
    dlg->exec();
    dlg->deleteLater();
}
