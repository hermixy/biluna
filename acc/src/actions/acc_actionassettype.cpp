/*****************************************************************
 * $Id: acc_actionassettype.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Oct 24, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionassettype.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionAssetType::ACC_ActionAssetType()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionAssetType::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionAssetType::getName(),
                              //QIcon(":/images/icons/star.png"),
                              tr("Asset &Types"));
        ga->setStatusTip(tr("Asset type maintenance"));
        ga->setFactory(ACC_ActionAssetType::factory);
    }
    return ga;
}

RB_Action* ACC_ActionAssetType::factory() {
    RB_Action* a = new ACC_ActionAssetType();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionAssetType::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogAssetType);
    dlg->exec();
    dlg->deleteLater();
}
