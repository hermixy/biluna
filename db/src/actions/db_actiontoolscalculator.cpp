/*****************************************************************
 * $Id: db_actiontoolscalculator.cpp 1806 2012-11-29 15:18:34Z rutger $
 * Created: Jul 13, 2011 3:14:42 PM - rutger
 *
 * Copyright (C) 201! Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontoolscalculator.h"

#include "db_dialogfactory.h"
#include "rb_dockwidget.h"
#include "rb_settings.h"


DB_ActionToolsCalculator::DB_ActionToolsCalculator() : RB_Action() {}


RB_GuiAction* DB_ActionToolsCalculator::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(DB_ActionToolsCalculator::getName(),
                                  QIcon(":/images/icons/calculator.png"),
                                  tr("&Calculator"));
        action->setStatusTip(tr("Open calculator"));
        action->setFactory(DB_ActionToolsCalculator::factory);
    }                         
    return action;
}

RB_Action* DB_ActionToolsCalculator::factory() {
    RB_Action* a = new DB_ActionToolsCalculator();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionToolsCalculator::trigger() {
    // No preliminary checks required
    // No model preparation

    // Get active dockwidget
    RB_DockWidget* dw = DB_DIALOGFACTORY->getDockWidget(
                DB_DialogFactory::WidgetCalculator,
                Qt::RightDockWidgetArea);
    dw->setAttribute(Qt::WA_DeleteOnClose, false); // otherwise perhaps true
    dw->show();
    dw->getWidget()->initFocus();
    // dockWin->raise();
}

