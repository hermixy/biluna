/*****************************************************************
 * $Id: acc_actiongltransreport.cpp 2131 2014-03-28 19:46:53Z rutger $
 * Created: Nov 9, 2010 10:59:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actiongltransreport.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionGlTransReport::ACC_ActionGlTransReport()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionGlTransReport::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionGlTransReport::getName(),
                              // QIcon(":/images/icons/star.png"),
                              tr("&GL transaction report"));
        ga->setStatusTip(tr("View GL transaction report"));
        ga->setFactory(ACC_ActionGlTransReport::factory);
    }
    return ga;
}

RB_Action* ACC_ActionGlTransReport::factory() {
    RB_Action* a = new ACC_ActionGlTransReport();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionGlTransReport::trigger() {
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
            ACC_DialogFactory::WidgetGlTransReport);
    mdiWin->show();
    mdiWin->raise();

    // show toolbars
    QToolBar* toolBar = ACC_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_edit");
    if (toolBar) {
        toolBar->show();
    }
}
