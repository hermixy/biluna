/*****************************************************************
 * $Id: acc_actionsettingsreport.cpp 2131 2014-03-28 19:46:53Z rutger $
 * Created: Nov 20, 2011 11:14:42 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionsettingsreport.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionSettingsReport::ACC_ActionSettingsReport()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionSettingsReport::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionSettingsReport::getName(),
                              //QIcon(":/images/icons/star.png"),
                              tr("S&ettings Report"));
        ga->setStatusTip(tr("Settings reports"));
        ga->setFactory(ACC_ActionSettingsReport::factory);
    }
    return ga;
}

RB_Action* ACC_ActionSettingsReport::factory() {
    RB_Action* a = new ACC_ActionSettingsReport();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionSettingsReport::trigger() {
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
            ACC_DialogFactory::WidgetSettingsReport);
    mdiWin->show();
    mdiWin->raise();

    // show toolbars
    QToolBar* toolBar = ACC_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_edit");
    if (toolBar) {
        toolBar->show();
    }
}
