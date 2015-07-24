/*****************************************************************
 * $Id: acc_actiontaxcategory.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Feb 22, 2010 9:14:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actiontaxcategory.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionTaxCategory::ACC_ActionTaxCategory()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionTaxCategory::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionTaxCategory::getName(),
                              //QIcon(":/images/icons/star.png"),
                              tr("Tax &Categories"));
        ga->setStatusTip(tr("Tax categories maintenance"));
        ga->setFactory(ACC_ActionTaxCategory::factory);
    }
    return ga;
}

RB_Action* ACC_ActionTaxCategory::factory() {
    RB_Action* a = new ACC_ActionTaxCategory();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionTaxCategory::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogTaxCategory);
    dlg->exec();
    dlg->deleteLater();
}
