/*****************************************************************
 * $Id: crm_actioncontact.cpp 1654 2012-05-30 19:10:24Z rutger $
 * Created: Feb 18, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_actioncontact.h"

#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"
#include "rb_mdiwindow.h"


CRM_ActionContact::CRM_ActionContact()
        : RB_Action() {
    // nothing
}


RB_GuiAction* CRM_ActionContact::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(CRM_ActionContact::getName(),
                              QIcon(":/images/icons/group_link_crm.png"),
                              tr("&Contacts"));
        ga->setStatusTip(tr("Edit/view contacts"));
        ga->setFactory(CRM_ActionContact::factory);
    }
    return ga;
}

RB_Action* CRM_ActionContact::factory() {
    RB_Action* a = new CRM_ActionContact();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void CRM_ActionContact::trigger() {
    // Check required settings
    if (CRM_MODELFACTORY->getRootId() == ""
            || !CRM_MODELFACTORY->getDatabase().isOpen()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create widget and corresponding dockwidget if applicable
    RB_MdiWindow* mdiWin = CRM_DIALOGFACTORY->getMdiWindow(
            CRM_DialogFactory::WidgetContact);
    mdiWin->show();
    mdiWin->raise();
}
