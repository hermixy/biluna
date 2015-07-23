/*****************************************************************
 * $Id: db_actionsystemdatabasemapping.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Nov 26, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystemdatabasemapping.h"

#include "db_dialogfactory.h"
#include "db_modelfactory.h"


DB_ActionSystemDatabaseMapping::DB_ActionSystemDatabaseMapping()
        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionSystemDatabaseMapping::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionSystemDatabaseMapping::getName(),
                                  "dialog",
                                  tr("Map/&Import Data"));
//        ga->setStatusTip(tr("Connect to database"));
//        ga->setCommand("da, database");
//        ga->setKeycode("daa");
//        ga->setStatusTip(tr("Connect to database ..."));
//        ga->setShortcut("Ctrl+B");
//        ga->setToolTip("Connect to database");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionSystemDatabaseMapping::factory);
    }
    return ga;
}

RB_Action* DB_ActionSystemDatabaseMapping::factory() {
    RB_Action* a = new DB_ActionSystemDatabaseMapping();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionSystemDatabaseMapping::trigger() {
    // No preliminary checks required

    // Create dialog
    RB_Dialog* dlg = DB_DIALOGFACTORY->getDialog(DB_DialogFactory::DialogDbMapping);
    dlg->exec();
    dlg->deleteLater();
}
