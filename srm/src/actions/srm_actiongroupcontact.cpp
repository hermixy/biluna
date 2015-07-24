/*****************************************************************
 * $Id: srm_actiongroupcontact.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Mar 5, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_actiongroupcontact.h"

#include "srm_dialogfactory.h"
#include "srm_modelfactory.h"
#include "rb_mdiwindow.h"


SRM_ActionGroupContact::SRM_ActionGroupContact()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SRM_ActionGroupContact::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SRM_ActionGroupContact::getName(),
//                              QIcon(":/images/icons/page_white_gear.png"),
                              tr("&Group && Contact"));
        ga->setStatusTip(tr("Group contact"));
        ga->setFactory(SRM_ActionGroupContact::factory);
    }
    return ga;
}

RB_Action* SRM_ActionGroupContact::factory() {
    RB_Action* a = new SRM_ActionGroupContact();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SRM_ActionGroupContact::trigger() {
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
            SRM_DialogFactory::WidgetGroupContact);

    mdiWin->show();
    mdiWin->raise();
}
