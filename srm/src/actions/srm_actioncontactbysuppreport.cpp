/*****************************************************************
 * $Id: srm_actioncontactbysuppreport.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Jun 8, 2012 21:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_actioncontactbysuppreport.h"

#include "db_actionfactory.h"
#include "rb_mdiwindow.h"
#include "srm_dialogfactory.h"
#include "srm_modelfactory.h"


SRM_ActionContactBySuppReport::SRM_ActionContactBySuppReport()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SRM_ActionContactBySuppReport::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SRM_ActionContactBySuppReport::getName(),
                              // QIcon(":/images/icons/group_edit_crm.png"),
                              tr("Contact b&y Customer Report"));
        ga->setStatusTip(tr("View contact by customer report"));
        ga->setFactory(SRM_ActionContactBySuppReport::factory);
    }
    return ga;
}

RB_Action* SRM_ActionContactBySuppReport::factory() {
    RB_Action* a = new SRM_ActionContactBySuppReport();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SRM_ActionContactBySuppReport::trigger() {
    // Check required settings
    if (SRM_MODELFACTORY->getRootId() == ""
            || !SRM_MODELFACTORY->getDatabase().isOpen()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Register widget with toolbar
    DB_ACTIONFACTORY->enableEditMenu(SRM_DialogFactory::WidgetContactBySuppReport);
    // DB_ACTIONFACTORY->enableFormatMenu(mdiWin->getWidgetType());

    // Show toolbars
    QToolBar* toolBar = SRM_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_edit");
    if (toolBar) {
        toolBar->show();
    }

    // Create widget
    RB_MdiWindow* mdiWin = SRM_DIALOGFACTORY->getMdiWindow(
            SRM_DialogFactory::WidgetContactBySuppReport);

    mdiWin->show();
    mdiWin->raise();

}
