/*****************************************************************
 * $Id: peng_actionpaint.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Mar 23, 2014 4:14:42 PM - rutger
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_actionpaint.h"

#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "rb_dialog.h"


PENG_ActionPaint::PENG_ActionPaint()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PENG_ActionPaint::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PENG_ActionPaint::getName(),
                              // QIcon(":/images/icons/equipmenttype_edit.png"),
                              tr("&Paint System"));
        ga->setStatusTip(tr("Edit paint systems"));
        ga->setFactory(PENG_ActionPaint::factory);
    }
    return ga;
}

RB_Action* PENG_ActionPaint::factory() {
    RB_Action* a = new PENG_ActionPaint();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void PENG_ActionPaint::trigger() {
    // Check required settings
    if (PENG_MODELFACTORY->getRootId() == ""
            || !PENG_MODELFACTORY->getDatabase().isOpen()) {
        PENG_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create widget
    // Create dialog
    RB_Dialog* dlg = PENG_DIALOGFACTORY->getDialog(PENG_DialogFactory::DialogPaint);
    dlg->exec();
    dlg->deleteLater();
}
