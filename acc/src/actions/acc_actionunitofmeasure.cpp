/*****************************************************************
 * $Id: acc_actionunitofmeasure.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 20, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionunitofmeasure.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionUnitOfMeasure::ACC_ActionUnitOfMeasure()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionUnitOfMeasure::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionUnitOfMeasure::getName(),
                              QIcon(":/images/icons/brick_uom.png"),
                              tr("&Unit of Measurement"));
        ga->setStatusTip(tr("Unit of measurement"));
        ga->setFactory(ACC_ActionUnitOfMeasure::factory);
    }
    return ga;
}

RB_Action* ACC_ActionUnitOfMeasure::factory() {
    RB_Action* a = new ACC_ActionUnitOfMeasure();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionUnitOfMeasure::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Prepare model(s)
    RB_MmProxy* model = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelUnitOfMeasure);
    model->setRoot(ACC_MODELFACTORY->getRootId());

    // Create dialog
    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogUnitOfMeasure);
    dlg->exec();
    dlg->deleteLater();
}
