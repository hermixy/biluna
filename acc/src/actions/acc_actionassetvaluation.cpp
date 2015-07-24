/*****************************************************************
 * $Id: acc_actionassetvaluation.cpp 1479 2011-11-17 15:51:46Z rutger $
 * Created: Nov 7, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionassetvaluation.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionAssetValuation::ACC_ActionAssetValuation()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionAssetValuation::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionAssetValuation::getName(),
                              //QIcon(":/images/icons/house.png"),
                              tr("Asset &Valuation"));
        ga->setStatusTip(tr("Asset data"));
        ga->setFactory(ACC_ActionAssetValuation::factory);
    }
    return ga;
}

RB_Action* ACC_ActionAssetValuation::factory() {
    RB_Action* a = new ACC_ActionAssetValuation();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionAssetValuation::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create widget
    RB_MdiWindow* mdiWin = ACC_DIALOGFACTORY->getMdiWindow(
            ACC_DialogFactory::WidgetAssetValuation);

    mdiWin->show();
    mdiWin->raise();
}
