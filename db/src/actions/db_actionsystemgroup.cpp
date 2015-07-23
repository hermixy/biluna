/*****************************************************************
 * $Id: db_actionsystemgroup.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Dec 18, 2014 12:26:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystemgroup.h"

#include "db_dialogfactory.h"
#include "db_modelfactory.h"


DB_ActionSystemGroup::DB_ActionSystemGroup()
        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionSystemGroup::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionSystemGroup::getName(),
//                                  "dialog",
                                  tr("System &group"));
//        ga->setCommand("da, database");
//        ga->setKeycode("daa");
//        ga->setStatusTip(tr("Connect to database"));
//        ga->setShortcut("Ctrl+B");
//        ga->setToolTip("Connect to database");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionSystemGroup::factory);
    }
    return ga;
}

RB_Action* DB_ActionSystemGroup::factory() {
    RB_Action* a = new DB_ActionSystemGroup();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionSystemGroup::trigger() {
    // Check required settings
    if (DB_MODELFACTORY->getRootId() == ""
            || !DB_MODELFACTORY->getDatabase().isOpen()) {
        DB_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = DB_DIALOGFACTORY->getDialog(DB_DialogFactory::DialogSystemGroup);
    dlg->exec();
    dlg->deleteLater();
}
