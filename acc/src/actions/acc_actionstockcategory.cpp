/*****************************************************************
 * $Id: acc_actionstockcategory.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Jan 17, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionstockcategory.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionStockCategory::ACC_ActionStockCategory()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionStockCategory::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionStockCategory::getName(),
//                              QIcon(":/images/icons/bricks.png"),
                              tr("Stock C&ategory"));
        ga->setStatusTip(tr("Stock Category and Items"));
        ga->setFactory(ACC_ActionStockCategory::factory);
    }
    return ga;
}

RB_Action* ACC_ActionStockCategory::factory() {
    RB_Action* a = new ACC_ActionStockCategory();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionStockCategory::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create widget
    RB_MdiWindow* mdiWin = ACC_DIALOGFACTORY->getMdiWindow(
            ACC_DialogFactory::WidgetStockCategory);
    mdiWin->show();
    mdiWin->raise();
}
