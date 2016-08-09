/*****************************************************************
 * $Id: db_actionfilenewspreadsheet.h 2016-08-09 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionfilenewspreadsheet.h"

#include "db_dialogfactory.h"
#include "db_textwidget.h"
#include "rb_mdiwindow.h"

DB_ActionFileNewSpreadSheet::DB_ActionFileNewSpreadSheet() : RB_Action() {}


RB_GuiAction* DB_ActionFileNewSpreadSheet::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(DB_ActionFileNewSpreadSheet::getName(),
                                  QIcon(":/images/icons/page_excel.png"),
                                  tr("Sp&readsheet"));
        action->setStatusTip(tr("Creates a new spreadsheet"));
//        action->setShortcut("Ctrl+N");
        action->setFactory(DB_ActionFileNewSpreadSheet::factory);
    }                         
    return action;
}

RB_Action* DB_ActionFileNewSpreadSheet::factory() {
    RB_Action* a = new DB_ActionFileNewSpreadSheet();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionFileNewSpreadSheet::trigger() {
    // No preliminary checks required

    // No model preparation

    // Create widget with new document name
    ++mUntitledNumber;
    RB_String docName = tr("Untitled") + RB_String::number(mUntitledNumber);

    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMdiWindow(
            DB_DialogFactory::WidgetSpreadSheet, docName, true);
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

    // TODO: show spreadsheet and relevant formatting toolbars
//    QToolBar* toolBar = DB_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_edit");
//    if (toolBar) {
//        toolBar->show();
//    }
//    toolBar = DB_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_edit_html");
//    if (toolBar) {
//        toolBar->show();
//    }
//    toolBar = DB_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_format");
//    if (toolBar) {
//        toolBar->show();
//    }
}

