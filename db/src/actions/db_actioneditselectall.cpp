/*****************************************************************
 * $Id: db_actioneditselectall.cpp 1092 2010-02-10 09:52:33Z rutger $
 * Created: Jan 8, 2010 6:07:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actioneditselectall.h"

#include "db_dialogfactory.h"
#include "rb_mdiwindow.h"
#include "rb_widget.h"

DB_ActionEditSelectAll::DB_ActionEditSelectAll() : RB_Action() {}


RB_GuiAction* DB_ActionEditSelectAll::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionEditSelectAll::getName(),
                                  QIcon(":/images/icons/page_selectall.png"),
                                  tr("Select &All"));
        ga->setStatusTip(tr("Select all"));
        ga->setShortcut("Ctrl+A");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionEditSelectAll::factory);
    }
    return ga;
}

RB_Action* DB_ActionEditSelectAll::factory() {
    RB_Action* a = new DB_ActionEditSelectAll();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionEditSelectAll::trigger() {
    // No preliminary checks required

    // No models to prepare

    // Get active widget
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getActiveMdiWindow();
    RB_Widget* wdgt = NULL;
    if (mdiWin) {
        wdgt = mdiWin->getWidget();
    } else {
        wdgt = mWidget;
    }

    if (wdgt) {
        // wdgt->init(); not required
        wdgt->editSelectAll();
    }
}
