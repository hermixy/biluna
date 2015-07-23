/*****************************************************************
 * $Id: db_actionfilesave.cpp 2217 2015-02-13 18:18:14Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionfilesave.h"

#include "db_dialogfactory.h"
#include "rb_mdiwindow.h"
#include "rb_settings.h"
#include "rb_widget.h"


DB_ActionFileSave::DB_ActionFileSave() : RB_Action() {

}


RB_GuiAction* DB_ActionFileSave::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionFileSave::getName(),
                                  QIcon(":/images/icons/disk.png"),
                                  tr("&Save"));
        ga->setShortcut("Ctrl+S");
        ga->setStatusTip(tr("Saves the current document"));
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
        ga->setFactory(DB_ActionFileSave::factory);
    }
    return ga;
}

RB_Action* DB_ActionFileSave::factory() {
    RB_Action* a = new DB_ActionFileSave();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionFileSave::trigger() {
    // No preliminary checks required

    // No models to prepare

    // Get active widget
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getActiveMdiWindow();
    if (!mdiWin) return;

    RB_Widget* wdgt = mdiWin->getWidget();
    if (!wdgt) {
        // widget set from outside
        wdgt = mWidget;
    }

    if (wdgt) {
        // wdgt->init(); not required
        QApplication::setOverrideCursor(Qt::WaitCursor);
        wdgt->fileSave(true);
        DB_DIALOGFACTORY->commandMessage("File save: " + wdgt->getSaveAsFileName());
        QApplication::restoreOverrideCursor();
    }
}

