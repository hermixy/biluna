/*****************************************************************
 * $Id: srm_actionsupplier.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Feb 18, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_actionsupplier.h"

#include "srm_dialogfactory.h"
#include "srm_modelfactory.h"
#include "rb_mdiwindow.h"


SRM_ActionSupplier::SRM_ActionSupplier()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SRM_ActionSupplier::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SRM_ActionSupplier::getName(),
                              QIcon(":/images/icons/group_gear_srm.png"),
                              tr("S&upplier"));
        ga->setStatusTip(tr("Edit/view supplier"));
        ga->setFactory(SRM_ActionSupplier::factory);
    }
    return ga;
}

RB_Action* SRM_ActionSupplier::factory() {
    RB_Action* a = new SRM_ActionSupplier();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SRM_ActionSupplier::trigger() {
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
            SRM_DialogFactory::WidgetSupplier);

    mdiWin->show();
    mdiWin->raise();
}
