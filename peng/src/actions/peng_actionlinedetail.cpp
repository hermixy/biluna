/*****************************************************************
 * $Id: peng_actionlinedetail.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Feb 16, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_actionlinedetail.h"

#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "rb_mdiwindow.h"


PENG_ActionLineDetail::PENG_ActionLineDetail()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PENG_ActionLineDetail::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PENG_ActionLineDetail::getName(),
                              QIcon(":/images/icons/pipeline.png"),
                              tr("Pipe Line"));
        ga->setStatusTip(tr("Line designation table detail"));
        ga->setFactory(PENG_ActionLineDetail::factory);
    }
    return ga;
}

RB_Action* PENG_ActionLineDetail::factory() {
    RB_Action* a = new PENG_ActionLineDetail();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void PENG_ActionLineDetail::trigger() {
    // Check required settings
    if (PENG_MODELFACTORY->getRootId() == ""
            || !PENG_MODELFACTORY->getDatabase().isOpen()) {
        PENG_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create widget
    RB_MdiWindow* mdiWin = PENG_DIALOGFACTORY->getMdiWindow(
            PENG_DialogFactory::WidgetLineDetail);
    mdiWin->show();
    PENG_DIALOGFACTORY->getMainWindow()->setActiveMdiWindow(mdiWin);
}
