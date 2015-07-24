/*****************************************************************
 * $Id: srm_actionactivity.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_actionactivity.h"

#include "srm_dialogfactory.h"
#include "srm_modelfactory.h"
#include "rb_mdiwindow.h"


SRM_ActionActivity::SRM_ActionActivity()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SRM_ActionActivity::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SRM_ActionActivity::getName(),
                              QIcon(":/images/icons/page_white_gear_srm.png"),
                              tr("&Activity"));
        ga->setStatusTip(tr("Activity"));
        ga->setFactory(SRM_ActionActivity::factory);
    }
    return ga;
}

RB_Action* SRM_ActionActivity::factory() {
    RB_Action* a = new SRM_ActionActivity();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SRM_ActionActivity::trigger() {
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
            SRM_DialogFactory::WidgetActivity);

    mdiWin->show();
    mdiWin->raise();
}
