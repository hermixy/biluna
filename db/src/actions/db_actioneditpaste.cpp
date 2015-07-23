/*****************************************************************
 * $Id: db_actioneditpaste.cpp 1092 2010-02-10 09:52:33Z rutger $
 * Created: Jan 5, 2009 6:07:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actioneditpaste.h"

#include "db_dialogfactory.h"
#include "rb_mdiwindow.h"
#include "rb_widget.h"

DB_ActionEditPaste::DB_ActionEditPaste() : RB_Action() {}


RB_GuiAction* DB_ActionEditPaste::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionEditPaste::getName(),
                                  QIcon(":/images/icons/page_paste.png"),
                                  tr("&Paste"));
        ga->setStatusTip(tr("Pastes the clipboard contents"));
        ga->setShortcut("Ctrl+V");
        ga->setCommand("paste,ps");
        ga->setKeycode("ps");
//        ga->setToolTip("Test DB object");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionEditPaste::factory);
    }
    return ga;
}

RB_Action* DB_ActionEditPaste::factory() {
    RB_Action* a = new DB_ActionEditPaste();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionEditPaste::trigger() {
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
        wdgt->editPaste();
    }
}
