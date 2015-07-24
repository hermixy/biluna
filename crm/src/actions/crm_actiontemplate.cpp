/*****************************************************************
 * $Id: crm_actiontemplate.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Mar 6, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_actiontemplate.h"

#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"
#include "db_dialogfactory.h"
#include "rb_mdiwindow.h"


CRM_ActionTemplate::CRM_ActionTemplate()
        : RB_Action() {
    // nothing
}


RB_GuiAction* CRM_ActionTemplate::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(CRM_ActionTemplate::getName(),
                              // QIcon(":/images/icons/page_gear.png"),
                              tr("&Template"));
        ga->setStatusTip(tr("Template"));
        ga->setFactory(CRM_ActionTemplate::factory);
    }
    return ga;
}

RB_Action* CRM_ActionTemplate::factory() {
    RB_Action* a = new CRM_ActionTemplate();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void CRM_ActionTemplate::trigger() {
    // Check required settings
    if (CRM_MODELFACTORY->getRootId() == ""
            || !CRM_MODELFACTORY->getDatabase().isOpen()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create widget
    RB_MdiWindow* mdiWin = CRM_DIALOGFACTORY->getMdiWindow(
            CRM_DialogFactory::WidgetTemplate);

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
