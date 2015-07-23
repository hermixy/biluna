/*****************************************************************
 * $Id: db_actionsystemeditproject.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Dec 14, 2012 9:57:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystemeditproject.h"

#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "db_projectdialog.h"
#include "rb_database.h"
#include "rb_mainwindow.h"


DB_ActionSystemEditProject::DB_ActionSystemEditProject()
        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionSystemEditProject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionSystemEditProject::getName(),
                                  "dialog",
                                  tr("&Edit project"));
        ga->setFactory(DB_ActionSystemEditProject::factory);
    }
    return ga;
}

RB_Action* DB_ActionSystemEditProject::factory() {
    RB_Action* a = new DB_ActionSystemEditProject();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionSystemEditProject::trigger() {
    // Check required settings
    if (DB_MODELFACTORY->getRootId() == ""
            || !DB_MODELFACTORY->getDatabase().isOpen()) {
        DB_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = DB_DIALOGFACTORY->getDialog(DB_DialogFactory::DialogProjectEdit);
    dlg->exec();
    dlg->deleteLater();
}
