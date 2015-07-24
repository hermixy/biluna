/*****************************************************************
 * $Id: peng_actionweldvolcalc.cpp 1867 2013-01-12 11:03:44Z rutger $
 * Created: Apr 15, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_actionweldvolcalc.h"

#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "rb_mdiwindow.h"


PENG_ActionWeldVolCalc::PENG_ActionWeldVolCalc()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PENG_ActionWeldVolCalc::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PENG_ActionWeldVolCalc::getName(),
                              // QIcon(":/images/icons/page_white_gear.png"),
                              tr("&Weldolet Weld Volume"));
        ga->setStatusTip(tr("Weldolet Weld Volume"));
        ga->setFactory(PENG_ActionWeldVolCalc::factory);
    }
    return ga;
}

RB_Action* PENG_ActionWeldVolCalc::factory() {
    RB_Action* a = new PENG_ActionWeldVolCalc();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void PENG_ActionWeldVolCalc::trigger() {
    // Check required settings
    if (PENG_MODELFACTORY->getRootId() == ""
            || !PENG_MODELFACTORY->getDatabase().isOpen()) {
        PENG_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create widget and corresponding dockwidget if applicable
    RB_MdiWindow* mdiWin = PENG_DIALOGFACTORY->getMdiWindow(
            PENG_DialogFactory::WidgetWeldoletWeldVolume);
    mdiWin->show();
}
