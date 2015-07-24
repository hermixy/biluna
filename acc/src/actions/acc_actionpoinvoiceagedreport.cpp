/*****************************************************************
 * $Id: acc_actionpoinvoiceagedreport.cpp 2131 2014-03-28 19:46:53Z rutger $
 * Created: Dec 8, 2010 12:29:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionpoinvoiceagedreport.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"


ACC_ActionPoInvoiceAgedReport::ACC_ActionPoInvoiceAgedReport()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionPoInvoiceAgedReport::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionPoInvoiceAgedReport::getName(),
                              QIcon(":/images/icons/page_coins_delete.png"),
                              tr("&Purchase invoice aged report"));
        ga->setStatusTip(tr("View purchase invoice aged report"));
        ga->setFactory(ACC_ActionPoInvoiceAgedReport::factory);
    }
    return ga;
}

RB_Action* ACC_ActionPoInvoiceAgedReport::factory() {
    RB_Action* a = new ACC_ActionPoInvoiceAgedReport();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionPoInvoiceAgedReport::trigger() {
    // Check required settings
    if (ACC_MODELFACTORY->getRootId() == ""
            || !ACC_MODELFACTORY->getDatabase().isOpen()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create widget and corresponding dockwidget if applicable
    RB_MdiWindow* mdiWin = ACC_DIALOGFACTORY->getMdiWindow(
            ACC_DialogFactory::WidgetPoInvoiceAgedReport);
    mdiWin->show();
    mdiWin->raise();

    // show toolbars
    QToolBar* toolBar = ACC_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_edit");
    if (toolBar) {
        toolBar->show();
    }
}
