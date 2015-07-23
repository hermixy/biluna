/*****************************************************************
 * $Id: db_actioneditzoomout.cpp 2130 2014-03-26 20:51:55Z rutger $
 * Created: Mar 24, 2010 6:48:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actioneditzoomout.h"

#include "db_dialogfactory.h"
#include "rb_mdiwindow.h"
#include "rb_widget.h"

DB_ActionEditZoomOut::DB_ActionEditZoomOut() : RB_Action() {}


RB_GuiAction* DB_ActionEditZoomOut::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionEditZoomOut::getName(),
                                  QIcon(":/images/icons/magnifier_zoom_out.png"),
                                  tr("Zoom &Out"));
        ga->setStatusTip(tr("Zoom out"));
//        ga->setShortcut("Ctrl+Z");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionEditZoomOut::factory);
    }
    return ga;
}

RB_Action* DB_ActionEditZoomOut::factory() {
    RB_Action* a = new DB_ActionEditZoomOut();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionEditZoomOut::trigger() {
    // Get active widget
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getActiveMdiWindow();
    RB_Widget* wdgt = NULL;
    if (mdiWin) {
        wdgt = mdiWin->getWidget();
    } else {
        wdgt = mWidget;
    }

    if (wdgt) {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        wdgt->editZoomOut();
        QApplication::restoreOverrideCursor();
    }
}
