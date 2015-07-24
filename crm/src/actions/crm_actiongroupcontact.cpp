/*****************************************************************
 * $Id: crm_actiongroupcontact.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Mar 5, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_actiongroupcontact.h"

#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"
#include "rb_mdiwindow.h"


CRM_ActionGroupContact::CRM_ActionGroupContact()
        : RB_Action() {
    // nothing
}


RB_GuiAction* CRM_ActionGroupContact::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(CRM_ActionGroupContact::getName(),
//                              QIcon(":/images/icons/page_white_gear.png"),
                              tr("&Group && Contact"));
        ga->setStatusTip(tr("Group contact"));
        ga->setFactory(CRM_ActionGroupContact::factory);
    }
    return ga;
}

RB_Action* CRM_ActionGroupContact::factory() {
    RB_Action* a = new CRM_ActionGroupContact();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void CRM_ActionGroupContact::trigger() {
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
            CRM_DialogFactory::WidgetGroupContact);

    mdiWin->show();
    mdiWin->raise();
}
