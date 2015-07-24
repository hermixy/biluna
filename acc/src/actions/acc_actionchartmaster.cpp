/*****************************************************************
 * $Id: acc_actionchartmaster.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Feb 25, 2010 10:00:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionchartmaster.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"


ACC_ActionChartMaster::ACC_ActionChartMaster()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionChartMaster::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionChartMaster::getName(),
                              //QIcon(":/images/icons/star.png"),
                              tr("C&hart of Accounts"));
        ga->setStatusTip(tr("Chart of accounts maintenance"));
        ga->setFactory(ACC_ActionChartMaster::factory);
    }
    return ga;
}

RB_Action* ACC_ActionChartMaster::factory() {
    RB_Action* a = new ACC_ActionChartMaster();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionChartMaster::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogChartMaster);
    dlg->exec();
    dlg->deleteLater();
}
