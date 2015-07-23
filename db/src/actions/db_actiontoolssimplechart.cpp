/*****************************************************************
 * $Id: db_actiontoolssimplechart.cpp 1785 2012-09-27 20:58:43Z rutger $
 * Created: Oct 10, 2011 8:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontoolssimplechart.h"

#include "db_dialogfactory.h"
#include "rb_mdiwindow.h"


DB_ActionToolsSimpleChart::DB_ActionToolsSimpleChart()
        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionToolsSimpleChart::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionToolsSimpleChart::getName(),
                              QIcon(":/images/icons/chart_pie_edit.png"),
                              tr("Simple chart types"));
        ga->setStatusTip(tr("Simple chart types"));
        ga->setFactory(DB_ActionToolsSimpleChart::factory);
    }
    return ga;
}

RB_Action* DB_ActionToolsSimpleChart::factory() {
    RB_Action* a = new DB_ActionToolsSimpleChart();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionToolsSimpleChart::trigger() {
    // Check required settings
    // Create widget
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMdiWindow(
                DB_DialogFactory::WidgetSimpleChart);
    mdiWin->show();
    mdiWin->raise();
}
