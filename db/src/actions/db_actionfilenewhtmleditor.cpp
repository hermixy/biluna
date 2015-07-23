/*****************************************************************
 * $Id: db_actionfilenewhtmleditor.cpp 2061 2014-01-12 16:34:14Z rutger $
 * Created: Oct 23, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionfilenewhtmleditor.h"

#include "db_dialogfactory.h"
#include "db_htmleditorwidget.h"
#include "rb_mdiwindow.h"

DB_ActionFileNewHtmlEditor::DB_ActionFileNewHtmlEditor() : RB_Action() {}


RB_GuiAction* DB_ActionFileNewHtmlEditor::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(DB_ActionFileNewHtmlEditor::getName(),
                                  QIcon(":/images/icons/page_white_code.png"),
                                  tr("&HTML Document"));
        action->setStatusTip(tr("Creates a new HTML Document"));
        action->setFactory(DB_ActionFileNewHtmlEditor::factory);
    }                         
    return action;
}

RB_Action* DB_ActionFileNewHtmlEditor::factory() {
    RB_Action* a = new DB_ActionFileNewHtmlEditor();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionFileNewHtmlEditor::trigger() {
    // No preliminary checks required

    // No model preparation

    // Create widget with new document name
    ++mUntitledNumber;
    RB_String docName = tr("Untitled") + RB_String::number(mUntitledNumber);

    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMdiWindow(
            DB_DialogFactory::WidgetHtmlEditor, docName, true);
    if (!mdiWin) return;

    DB_HtmlEditorWidget* wdgt = dynamic_cast<DB_HtmlEditorWidget*>(mdiWin->getWidget());

    if (!wdgt) {
        // widget set from outside
        wdgt = dynamic_cast<DB_HtmlEditorWidget*>(mWidget);
    }

    if (wdgt) {
        // wdgt->init(); not required
        wdgt->fileNew();
    }
    mdiWin->show();
    mdiWin->raise();

    // show toolbars
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

