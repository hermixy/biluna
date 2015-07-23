/*****************************************************************
 * $Id: db_actionfilenewplaintexteditor.cpp 2099 2014-02-17 19:48:13Z rutger $
 * Created: Jan 23, 2012 4:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionfilenewplaintexteditor.h"

#include "db_dialogfactory.h"
#include "db_plaintextwidget.h"
#include "rb_mdiwindow.h"

DB_ActionFileNewPlainTextEditor::DB_ActionFileNewPlainTextEditor() : RB_Action() {}


RB_GuiAction* DB_ActionFileNewPlainTextEditor::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(DB_ActionFileNewPlainTextEditor::getName(),
                                  QIcon(":/images/icons/page_white_text.png"),
                                  tr("&Plain Text Document"));
        action->setCommand("fnp");
        action->setKeycode("fnp");
        action->setStatusTip(tr("Creates a new plain text document"));
        action->setFactory(DB_ActionFileNewPlainTextEditor::factory);
    }                         
    return action;
}

RB_Action* DB_ActionFileNewPlainTextEditor::factory() {
    RB_Action* a = new DB_ActionFileNewPlainTextEditor();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionFileNewPlainTextEditor::trigger() {
    // No preliminary checks required

    // No model preparation

    // Create widget with new document name
    ++mUntitledNumber;
    RB_String docName = tr("Untitled") + RB_String::number(mUntitledNumber);

    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMdiWindow(
            DB_DialogFactory::WidgetPlainTextDocument, docName, true);
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
    // A new plain text window, without source window
    // needs to be set actively as modified.
    // The Rich Text and Html have source widgets
    // which will emit window modified.
    mdiWin->slotChangedModification(true);

    // show toolbars
    QToolBar* toolBar = DB_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_edit");
    if (toolBar) {
        toolBar->show();
    }
}

