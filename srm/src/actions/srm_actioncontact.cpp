/*****************************************************************
 * $Id: srm_actioncontact.cpp 2076 2014-01-27 16:02:14Z rutger $
 * Created: Feb 18, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_actioncontact.h"

#include "srm_dialogfactory.h"
#include "srm_modelfactory.h"
#include "rb_mdiwindow.h"


SRM_ActionContact::SRM_ActionContact()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SRM_ActionContact::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SRM_ActionContact::getName(),
                              QIcon(":/images/icons/group_link_srm.png"),
                              tr("&Contact"));
        ga->setStatusTip(tr("Edit/view contacts"));
        ga->setFactory(SRM_ActionContact::factory);
    }
    return ga;
}

RB_Action* SRM_ActionContact::factory() {
    RB_Action* a = new SRM_ActionContact();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SRM_ActionContact::trigger() {
    // Check required settings
    if (SRM_MODELFACTORY->getRootId() == ""
            || !SRM_MODELFACTORY->getDatabase().isOpen()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create widget and corresponding dockwidget if applicable
    RB_MdiWindow* mdiWin = SRM_DIALOGFACTORY->getMdiWindow(
            SRM_DialogFactory::WidgetContact);
    mdiWin->show();
    mdiWin->raise();
}
