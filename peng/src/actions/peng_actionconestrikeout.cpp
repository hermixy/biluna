/*****************************************************************
 * $Id: peng_actionconestrikeout.cpp 1867 2013-01-12 11:03:44Z rutger $
 * Created: Aug 10, 2011 8:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_actionconestrikeout.h"

#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "rb_mdiwindow.h"


PENG_ActionConeStrikeOut::PENG_ActionConeStrikeOut()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PENG_ActionConeStrikeOut::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PENG_ActionConeStrikeOut::getName(),
                              // QIcon(":/images/icons/page_white_gear.png"),
                              tr("&Cone Strikeout"));
        ga->setStatusTip(tr("Cone Strikeout"));
        ga->setFactory(PENG_ActionConeStrikeOut::factory);
    }
    return ga;
}

RB_Action* PENG_ActionConeStrikeOut::factory() {
    RB_Action* a = new PENG_ActionConeStrikeOut();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void PENG_ActionConeStrikeOut::trigger() {
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
            PENG_DialogFactory::WidgetConeStrikeOut);
    mdiWin->show();
}
