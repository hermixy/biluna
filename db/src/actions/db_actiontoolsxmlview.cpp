/*****************************************************************
 * $Id: db_actionfilenewplaintexteditor.h 1538 2012-01-25 23:37:38Z rutger $
 * Created: Nov 11, 2013 9:08:25 AM - Rutger Botermans
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontoolsxmlview.h"

#include "db_dialogfactory.h"
#include "db_xmlviewvalidatorwidget.h"
#include "rb_mdiwindow.h"

DB_ActionToolsXmlView::DB_ActionToolsXmlView() : RB_Action() {}


RB_GuiAction* DB_ActionToolsXmlView::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(DB_ActionToolsXmlView::getName(),
                                  QIcon(":/images/icons/xml.png"),
                                  tr("&XML view/validator Document"));
        action->setCommand("fnx");
        action->setKeycode("fnx");
        action->setStatusTip(tr("Creates a new XML view/validator document"));
        action->setFactory(DB_ActionToolsXmlView::factory);
    }
    return action;
}

RB_Action* DB_ActionToolsXmlView::factory() {
    RB_Action* a = new DB_ActionToolsXmlView();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionToolsXmlView::trigger() {
    // No preliminary checks required
    // No model preparation

    // Create widget with new document name
    ++mUntitledNumber;
    RB_String docName = tr("Untitled") + RB_String::number(mUntitledNumber);

    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMdiWindow(
            DB_DialogFactory::WidgetXmlViewValidator, docName);
    if (!mdiWin) return;

//    RB_Widget* wdgt = mdiWin->getWidget();

//    if (!wdgt) {
//        // widget set from outside
//        wdgt = mWidget;
//    }

//    if (wdgt) {
//        // wdgt->init(); not required
//        wdgt->fileNew();
//    }
    mdiWin->show();
    mdiWin->raise();

    // show toolbars
//    QToolBar* toolBar = DB_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_edit");
//    if (toolBar) {
//        toolBar->show();
//    }
}
