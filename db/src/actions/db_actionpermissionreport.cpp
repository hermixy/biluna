/*****************************************************************
 * $Id: db_actionpermissionreport.cpp 2141 2014-04-07 17:53:21Z rutger $
 * Created: Nov 21, 2015 6:57:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionpermissionreport.h"

#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "rb_mdiwindow.h"


DB_ActionPermissionReport::DB_ActionPermissionReport()
        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionPermissionReport::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionPermissionReport::getName(),
                              // QIcon(":/images/icons/star.png"),
                              tr("&Permission list report"));
        ga->setStatusTip(tr("View permission list report"));
        ga->setFactory(DB_ActionPermissionReport::factory);
    }
    return ga;
}

RB_Action* DB_ActionPermissionReport::factory() {
    RB_Action* a = new DB_ActionPermissionReport();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionPermissionReport::trigger() {
    // Check required settings
    if (!DB_MODELFACTORY->getDatabase().isOpen()) {
        DB_DIALOGFACTORY->requestWarningDialog(tr("Not connected to database.\n"
                                                   "First connect to database."));
        return;
    }

    // Create widget and corresponding dockwidget if applicable
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMdiWindow(
            DB_DialogFactory::WidgetPermissionReport);
    mdiWin->show();
    mdiWin->raise();

    // show toolbars
    QToolBar* toolBar = DB_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_edit");
    if (toolBar) {
        toolBar->show();
    }
}
