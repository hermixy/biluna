/*****************************************************************
 * $Id: db_actiontoolssimplereport.cpp 2200 2014-12-15 10:44:04Z rutger $
 * Created: Jul 13, 2011 3:14:42 PM - rutger
 *
 * Copyright (C) 201! Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontoolssimplereport.h"

#include "db_actionfileopenmultiple.h"
#include "db_dialogfactory.h"
#include "rb_mdiwindow.h"
#include "rb_settings.h"


DB_ActionToolsSimpleReport::DB_ActionToolsSimpleReport() : RB_Action() {}


RB_GuiAction* DB_ActionToolsSimpleReport::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(DB_ActionToolsSimpleReport::getName(),
                                  // QIcon(":/images/icons/application_view_list.png"),
                                  tr("Simple &Report"));
        action->setStatusTip(tr("Creates simple report from database"));
        action->setFactory(DB_ActionToolsSimpleReport::factory);
    }                         
    return action;
}

RB_Action* DB_ActionToolsSimpleReport::factory() {
    RB_Action* a = new DB_ActionToolsSimpleReport();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionToolsSimpleReport::trigger() {
    // No preliminary checks required
    // No model preparation

    // Get active MDI widget
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMdiWindow(
                DB_DialogFactory::WidgetSimpleReport);
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

