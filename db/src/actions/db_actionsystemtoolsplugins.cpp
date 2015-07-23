/*****************************************************************
 * $Id: db_actionsystemtoolsplugins.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystemtoolsplugins.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"


DB_ActionSystemToolsPlugins::DB_ActionSystemToolsPlugins() : RB_Action() {

}


RB_GuiAction* DB_ActionSystemToolsPlugins::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga == NULL) {
        ga = new RB_GuiAction(DB_ActionSystemToolsPlugins::getName(),
                                  // "fileclose",
                                  tr("&Plugins"));
        ga->setStatusTip(tr("Show status of available plugins"));
//        action->setShortcut("Ctrl+W");
        ga->setFactory(DB_ActionSystemToolsPlugins::factory);
    }                                  
    return ga;
}

RB_Action* DB_ActionSystemToolsPlugins::factory() {
    RB_Action* a = new DB_ActionSystemToolsPlugins();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionSystemToolsPlugins::trigger() {
    // Check required settings
//    if (DB_MODELFACTORY->getRootId() == ""
//            || !DB_MODELFACTORY->getDatabase().isOpen()) {
//        DB_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
//                                                   "Connect first to database\n"
//                                                   "and then select project."));
//        return;
//    }

    // Create dialog
    RB_Dialog* dlg = DB_DIALOGFACTORY->getDialog(DB_DialogFactory::DialogPluginTester);
    dlg->exec();
    dlg->deleteLater();
}

