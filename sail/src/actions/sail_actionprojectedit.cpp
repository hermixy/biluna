/*****************************************************************
 * $Id: sail_actionprojectedit.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_actionprojectedit.h"

#include "sail_dialogfactory.h"
#include "sail_modelfactory.h"
#include "rb_dialogwindow.h"


SAIL_ActionProjectEdit::SAIL_ActionProjectEdit()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SAIL_ActionProjectEdit::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SAIL_ActionProjectEdit::getName(),
                              //QIcon(":/images/icons/star.png"),
                              tr("&Project Edit"));
        ga->setStatusTip(tr("SAIL project settings maintenance"));
        ga->setFactory(SAIL_ActionProjectEdit::factory);
    }
    return ga;
}

RB_Action* SAIL_ActionProjectEdit::factory() {
    RB_Action* a = new SAIL_ActionProjectEdit();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SAIL_ActionProjectEdit::trigger() {
    // Check required settings
    if (SAIL_MODELFACTORY->getRootId() == ""
            || !SAIL_MODELFACTORY->getDatabase().isOpen()) {
        SAIL_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog, reuse Project dialog for selecting the root account
    RB_DialogWindow* dlg = SAIL_DIALOGFACTORY->getDialogWindow(
                SAIL_DialogFactory::WidgetProjectEdit);
    dlg->exec();
    dlg->deleteLater();

    // Make sure current company object (SAIL_Project) is up-to-date
    RB_ObjectBase* root = SAIL_MODELFACTORY->getRoot();

    if (!root) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "SAIL_ActionProjectEdit::trigger() "
                        "root is NULL ERROR");
    } else {
        root->dbRead(SAIL_MODELFACTORY->getDatabase(), RB2::ResolveNone);
    }
}
