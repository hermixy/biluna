/*****************************************************************
 * $Id: pcalc_actiondimension.cpp 0 2016-07-20 15:43:20Z rutger $
 * Created: July 20, 2016 10:43:42 AM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_actiondimension.h"

#include "pcalc_dialogfactory.h"
#include "pcalc_modelfactory.h"
#include "rb_dialogwindow.h"


PCALC_ActionDimension::PCALC_ActionDimension()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PCALC_ActionDimension::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PCALC_ActionDimension::getName(),
                              // QIcon(":/images/icons/page_white_gear.png"),
                              tr("&Dimension"));
        ga->setStatusTip(tr("Dimension values"));
        ga->setFactory(PCALC_ActionDimension::factory);
    }
    return ga;
}

RB_Action* PCALC_ActionDimension::factory() {
    RB_Action* a = new PCALC_ActionDimension();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void PCALC_ActionDimension::trigger() {
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
            PCALC_DialogFactory::WidgetDimension);
    dlg->exec();
    dlg->deleteLater();
}
