/*****************************************************************
 * $Id: db_actionsystemclearsettings.cpp 1391 2011-03-28 15:26:01Z rutger $
 * Created: Mar 28, 2011 3:48:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystemclearsettings.h"

#include "rb_settings.h"


DB_ActionSystemClearSettings::DB_ActionSystemClearSettings() : RB_Action() {}


RB_GuiAction* DB_ActionSystemClearSettings::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionSystemClearSettings::getName(),
//                                  QIcon(":/images/icons/page_copy.png"),
                                  tr("&Clear settings"));
        ga->setStatusTip(tr("Clear all application settings"));
//        ga->setShortcut("Ctrl+C");
        //        ga->setCommand("db object, db");
        //        ga->setKeycode("db");
        //        ga->setStatusTip(tr("Test DB object"));
        //        ga->setShortcut("Ctrl+O");
        //        ga->setToolTip("Test DB object");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionSystemClearSettings::factory);
    }
    return ga;
}

RB_Action* DB_ActionSystemClearSettings::factory() {
    RB_Action* a = new DB_ActionSystemClearSettings();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionSystemClearSettings::trigger() {
    // No preliminary checks required

    // No models to prepare

    // No active widget to get

    RB_SETTINGS->clear();
}
