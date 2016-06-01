/*****************************************************************
 * $Id: pcalc_actionmaterial.h 0 2016-06-01 15:43:20Z rutger $
 * Created: June 1, 2016 15:43:42 AM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_actionmaterial.h"

#include "pcalc_dialogfactory.h"
#include "pcalc_modelfactory.h"
#include "rb_dialogwindow.h"


PCALC_ActionMaterial::PCALC_ActionMaterial()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PCALC_ActionMaterial::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PCALC_ActionMaterial::getName(),
                              // QIcon(":/images/icons/page_white_gear.png"),
                              tr("M&aterial"));
        ga->setStatusTip(tr("Material values"));
        ga->setFactory(PCALC_ActionMaterial::factory);
    }
    return ga;
}

RB_Action* PCALC_ActionMaterial::factory() {
    RB_Action* a = new PCALC_ActionMaterial();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void PCALC_ActionMaterial::trigger() {
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
            PCALC_DialogFactory::WidgetMaterial);
    dlg->exec();
    dlg->deleteLater();
}
