/*****************************************************************
 * $Id: srm_actiontemplate.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Mar 6, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_actiontemplate.h"

#include "srm_dialogfactory.h"
#include "srm_modelfactory.h"
#include "db_dialogfactory.h"
#include "rb_mdiwindow.h"


SRM_ActionTemplate::SRM_ActionTemplate()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SRM_ActionTemplate::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SRM_ActionTemplate::getName(),
                              // QIcon(":/images/icons/page_gear.png"),
                              tr("&Template"));
        ga->setStatusTip(tr("Template"));
        ga->setFactory(SRM_ActionTemplate::factory);
    }
    return ga;
}

RB_Action* SRM_ActionTemplate::factory() {
    RB_Action* a = new SRM_ActionTemplate();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SRM_ActionTemplate::trigger() {
    // Check required settings
    if (SRM_MODELFACTORY->getRootId() == ""
            || !SRM_MODELFACTORY->getDatabase().isOpen()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create widget
    RB_MdiWindow* mdiWin = SRM_DIALOGFACTORY->getMdiWindow(
            SRM_DialogFactory::WidgetTemplate);

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
