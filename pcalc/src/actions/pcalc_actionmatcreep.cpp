/*****************************************************************
 * $Id: pcalc_actionmatcreep.cpp 0 2016-06-01 10:23:20Z rutger $
 * Created: June 1, 2016 10:23:42 AM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_actionmatcreep.h"

#include "pcalc_dialogfactory.h"
#include "pcalc_modelfactory.h"
#include "rb_dialogwindow.h"


PCALC_ActionMatCreep::PCALC_ActionMatCreep()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PCALC_ActionMatCreep::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PCALC_ActionMatCreep::getName(),
                              // QIcon(":/images/icons/page_white_gear.png"),
                              tr("C&reep"));
        ga->setStatusTip(tr("Material creep values"));
        ga->setFactory(PCALC_ActionMatCreep::factory);
    }
    return ga;
}

RB_Action* PCALC_ActionMatCreep::factory() {
    RB_Action* a = new PCALC_ActionMatCreep();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void PCALC_ActionMatCreep::trigger() {
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
            PCALC_DialogFactory::WidgetMatCreep);
    dlg->exec();
    dlg->deleteLater();
}
