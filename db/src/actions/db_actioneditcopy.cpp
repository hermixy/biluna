/*****************************************************************
 * $Id: db_actioneditcopy.cpp 1092 2010-02-10 09:52:33Z rutger $
 * Created: Jan 5, 2009 6:07:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actioneditcopy.h"

#include "db_dialogfactory.h"
#include "rb_mdiwindow.h"
#include "rb_widget.h"

DB_ActionEditCopy::DB_ActionEditCopy() : RB_Action() {}


RB_GuiAction* DB_ActionEditCopy::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionEditCopy::getName(),
                                  QIcon(":/images/icons/page_copy.png"),
                                  tr("&Copy"));
        ga->setStatusTip(tr("Copy selection to clipboard"));
        ga->setShortcut("Ctrl+C");
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
        ga->setFactory(DB_ActionEditCopy::factory);
    }
    return ga;
}

RB_Action* DB_ActionEditCopy::factory() {
    RB_Action* a = new DB_ActionEditCopy();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionEditCopy::trigger() {
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
        wdgt->editCopy();
    }
}
