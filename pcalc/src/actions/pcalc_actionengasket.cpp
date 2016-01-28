/*****************************************************************
 * $Id: pcalc_actionen1591flange.cpp 2234 2015-04-30 19:23:20Z rutger $
 * Created: Sept 15, 2015 4:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_actionengasket.h"

#include "pcalc_dialogfactory.h"
#include "pcalc_modelfactory.h"
#include "rb_dialogwindow.h"


PCALC_ActionENGasket::PCALC_ActionENGasket()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PCALC_ActionENGasket::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PCALC_ActionENGasket::getName(),
                              // QIcon(":/images/icons/page_white_gear.png"),
                              tr("EN &gasket"));
        ga->setStatusTip(tr("EN gasket"));
        ga->setFactory(PCALC_ActionENGasket::factory);
    }
    return ga;
}

RB_Action* PCALC_ActionENGasket::factory() {
    RB_Action* a = new PCALC_ActionENGasket();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void PCALC_ActionENGasket::trigger() {
    // Check required settings
    if (PCALC_MODELFACTORY->getRootId() == ""
            || !PCALC_MODELFACTORY->getDatabase().isOpen()) {
        PCALC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create widget and corresponding dockwidget if applicable
    RB_DialogWindow* dlg = PCALC_DIALOGFACTORY->getDialogWindow(
            PCALC_DialogFactory::WidgetEN13555Gasket);
    dlg->exec();
}
