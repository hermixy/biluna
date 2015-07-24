/*****************************************************************
 * $Id: srm_actioncampaigntarget.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Mar 5, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_actioncampaigntarget.h"

#include "srm_dialogfactory.h"
#include "srm_modelfactory.h"
#include "rb_mdiwindow.h"


SRM_ActionCampaignTarget::SRM_ActionCampaignTarget()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SRM_ActionCampaignTarget::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SRM_ActionCampaignTarget::getName(),
                              QIcon(":/images/icons/flag_red.png"),
                              tr("Campai&gn"));
        ga->setStatusTip(tr("Campaign Target"));
        ga->setFactory(SRM_ActionCampaignTarget::factory);
    }
    return ga;
}

RB_Action* SRM_ActionCampaignTarget::factory() {
    RB_Action* a = new SRM_ActionCampaignTarget();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SRM_ActionCampaignTarget::trigger() {
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
            SRM_DialogFactory::WidgetCampaignTarget);

    mdiWin->show();
    mdiWin->raise();
}
