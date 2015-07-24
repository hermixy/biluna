/*****************************************************************
 * $Id: sail_actionpolardiagram.cpp 1979 2013-08-22 19:56:38Z rutger $
 * Created: Aug 22, 2013 9:07:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_actionpolardiagram.h"

#include "rb_mdiwindow.h"
#include "sail_dialogfactory.h"
#include "sail_modelfactory.h"


SAIL_ActionPolarDiagram::SAIL_ActionPolarDiagram()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SAIL_ActionPolarDiagram::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SAIL_ActionPolarDiagram::getName(),
                                  tr("P&olar Diagram"));
        ga->setFactory(SAIL_ActionPolarDiagram::factory);
    }
    return ga;
}

RB_Action* SAIL_ActionPolarDiagram::factory() {
    RB_Action* a = new SAIL_ActionPolarDiagram();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SAIL_ActionPolarDiagram::trigger() {
    // Check required settings
    if (SAIL_MODELFACTORY->getRootId() == ""
            || !SAIL_MODELFACTORY->getDatabase().isOpen()) {
        SAIL_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create widget
    RB_MdiWindow* mdiWin = SAIL_DIALOGFACTORY->getMdiWindow(
            SAIL_DialogFactory::WidgetPolarDiagram);

    mdiWin->show();
    mdiWin->raise();
}
