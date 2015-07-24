/*****************************************************************
 * $Id: acc_actionallocnreport.cpp 2186 2014-10-16 14:04:01Z rutger $
 * Created: Jun 30, 2011 7:57:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionallocnreport.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionAllocnReport::ACC_ActionAllocnReport()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionAllocnReport::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionAllocnReport::getName(),
                              // QIcon(":/images/icons/star.png"),
                              tr("&Allocation/reconciliation report"));
        ga->setStatusTip(tr("View allocation/reconciliation report"));
        ga->setFactory(ACC_ActionAllocnReport::factory);
    }
    return ga;
}

RB_Action* ACC_ActionAllocnReport::factory() {
    RB_Action* a = new ACC_ActionAllocnReport();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionAllocnReport::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create widget and corresponding dockwidget if applicable
    RB_MdiWindow* mdiWin = ACC_DIALOGFACTORY->getMdiWindow(
            ACC_DialogFactory::WidgetAllocnReport);
    mdiWin->show();
    mdiWin->raise();

    // show toolbars
    QToolBar* toolBar = ACC_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_edit");
    if (toolBar) {
        toolBar->show();
    }
}
