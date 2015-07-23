/*****************************************************************
 * $Id: db_actionfilenew.cpp 2061 2014-01-12 16:34:14Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionfilenew.h"

#include "db_dialogfactory.h"
#include "db_textwidget.h"
#include "rb_mdiwindow.h"

DB_ActionFileNew::DB_ActionFileNew() : RB_Action() {}


RB_GuiAction* DB_ActionFileNew::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(DB_ActionFileNew::getName(),
                                  QIcon(":/images/icons/page_word.png"),
                                  tr("Rich &Text Document"));
        action->setStatusTip(tr("Creates a new rich text document"));
        action->setShortcut("Ctrl+N");
        action->setFactory(DB_ActionFileNew::factory);
    }                         
    return action;
}

RB_Action* DB_ActionFileNew::factory() {
    RB_Action* a = new DB_ActionFileNew();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionFileNew::trigger() {
    // No preliminary checks required

    // No model preparation

    // Create widget with new document name
    ++mUntitledNumber;
    RB_String docName = tr("Untitled") + RB_String::number(mUntitledNumber);

    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMdiWindow(
            DB_DialogFactory::WidgetTextDocument, docName, true);
    if (!mdiWin) return;

    RB_Widget* wdgt = mdiWin->getWidget();

    if (!wdgt) {
        // widget set from outside
        wdgt = mWidget;
    }

    if (wdgt) {
        // wdgt->init(); not required
        wdgt->fileNew();
    }
    mdiWin->show();
    mdiWin->raise();

    // show formatting toolbar
    QToolBar* toolBar = DB_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_edit");
    if (toolBar) {
        toolBar->show();
    }
    toolBar = DB_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_edit_html");
    if (toolBar) {
        toolBar->show();
    }
    toolBar = DB_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_format");
    if (toolBar) {
        toolBar->show();
    }
}

