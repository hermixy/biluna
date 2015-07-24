/*****************************************************************
 * $Id: srm_actionprojectedit.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_actionprojectedit.h"

#include "srm_dialogfactory.h"
#include "srm_modelfactory.h"
#include "rb_mdiwindow.h"


SRM_ActionProjectEdit::SRM_ActionProjectEdit()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SRM_ActionProjectEdit::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SRM_ActionProjectEdit::getName(),
                              //QIcon(":/images/icons/star.png"),
                              tr("&Account Edit"));
        ga->setStatusTip(tr("SRM account settings maintenance"));
        ga->setFactory(SRM_ActionProjectEdit::factory);
    }
    return ga;
}

RB_Action* SRM_ActionProjectEdit::factory() {
    RB_Action* a = new SRM_ActionProjectEdit();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SRM_ActionProjectEdit::trigger() {
    // Check required settings
    if (SRM_MODELFACTORY->getRootId() == ""
            || !SRM_MODELFACTORY->getDatabase().isOpen()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog, reuse Project dialog for selecting the root account
    RB_Dialog* dlg = SRM_DIALOGFACTORY->getDialog(SRM_DialogFactory::DialogProjectEdit);
    dlg->exec();
    dlg->deleteLater();

    // Make sure current company object (SRM_Project) is up-to-date
    RB_ObjectBase* root = SRM_MODELFACTORY->getRoot();

    if (!root) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "srm_ActionProjectEdit::trigger() "
                        "root is NULL ERROR");
    } else {
        root->dbRead(SRM_MODELFACTORY->getDatabase(), RB2::ResolveNone);
    }
}
