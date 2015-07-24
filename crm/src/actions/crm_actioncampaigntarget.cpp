/*****************************************************************
 * $Id: crm_actioncampaigntarget.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Mar 5, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_actioncampaigntarget.h"

#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"
#include "rb_mdiwindow.h"


CRM_ActionCampaignTarget::CRM_ActionCampaignTarget()
        : RB_Action() {
    // nothing
}


RB_GuiAction* CRM_ActionCampaignTarget::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(CRM_ActionCampaignTarget::getName(),
                              QIcon(":/images/icons/flag_blue.png"),
                              tr("Campai&gn"));
        ga->setStatusTip(tr("Campaign Target"));
        ga->setFactory(CRM_ActionCampaignTarget::factory);
    }
    return ga;
}

RB_Action* CRM_ActionCampaignTarget::factory() {
    RB_Action* a = new CRM_ActionCampaignTarget();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void CRM_ActionCampaignTarget::trigger() {
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
            CRM_DialogFactory::WidgetCampaignTarget);

    mdiWin->show();
    mdiWin->raise();
}
