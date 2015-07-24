/*****************************************************************
 * $Id: srm_actionactivityreport.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Jun 8, 2012 21:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_actionactivityreport.h"

#include "db_actionfactory.h"
#include "rb_mdiwindow.h"
#include "srm_dialogfactory.h"
#include "srm_modelfactory.h"


SRM_ActionActivityReport::SRM_ActionActivityReport()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SRM_ActionActivityReport::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SRM_ActionActivityReport::getName(),
                              // QIcon(":/images/icons/group_edit_crm.png"),
                              tr("&Activity Report"));
        ga->setStatusTip(tr("View activity report"));
        ga->setFactory(SRM_ActionActivityReport::factory);
    }
    return ga;
}

RB_Action* SRM_ActionActivityReport::factory() {
    RB_Action* a = new SRM_ActionActivityReport();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SRM_ActionActivityReport::trigger() {
    // Check required settings
    if (SRM_MODELFACTORY->getRootId() == ""
            || !SRM_MODELFACTORY->getDatabase().isOpen()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Register widget with toolbar
    DB_ACTIONFACTORY->enableEditMenu(SRM_DialogFactory::WidgetActivityReport);
    // DB_ACTIONFACTORY->enableFormatMenu(mdiWin->getWidgetType());

    // Show toolbars
    QToolBar* toolBar = SRM_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_edit");
    if (toolBar) {
        toolBar->show();
    }

    // Create widget
    RB_MdiWindow* mdiWin = SRM_DIALOGFACTORY->getMdiWindow(
            SRM_DialogFactory::WidgetActivityReport);

    mdiWin->show();
    mdiWin->raise();
}
