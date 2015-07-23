/*****************************************************************
 * $Id: db_actionsystempermission.cpp 2126 2014-03-20 15:16:16Z rutger $
 * Created: Dec 2, 2013 10:14:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystempermission.h"

#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "rb_mdiwindow.h"


DB_ActionSystemPermission::DB_ActionSystemPermission()
                        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionSystemPermission::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionSystemPermission::getName(),
                              //QIcon(":/images/icons/star.png"),
                              tr("User pe&rmission"));
        ga->setStatusTip(tr("User permission edit window"));
        ga->setFactory(DB_ActionSystemPermission::factory);
    }
    return ga;
}

RB_Action* DB_ActionSystemPermission::factory() {
    RB_Action* a = new DB_ActionSystemPermission();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionSystemPermission::trigger() {
    // Check required settings
    if (DB_MODELFACTORY->getRootId() == ""
            || !DB_MODELFACTORY->getDatabase().isOpen()) {
        DB_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMdiWindow(
            DB_DialogFactory::WidgetPermission);
    if (!mdiWin) return;
    mdiWin->show();
    mdiWin->raise();

    QApplication::restoreOverrideCursor();
}
