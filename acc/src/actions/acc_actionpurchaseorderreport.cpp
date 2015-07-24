/*****************************************************************
 * $Id: acc_actionpurchaseorderreport.cpp 2131 2014-03-28 19:46:53Z rutger $
 * Created: Feb 22, 2012 17:00:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionpurchaseorderreport.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_orderreportwidget.h"
#include "rb_mdiwindow.h"


ACC_ActionPurchaseOrderReport::ACC_ActionPurchaseOrderReport()
        : RB_Action() {
    // nothing
}


RB_GuiAction* ACC_ActionPurchaseOrderReport::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionPurchaseOrderReport::getName(),
                              //QIcon(":/images/icons/report_coins.png"),
                              tr("&Purchase Order Report"));
        ga->setStatusTip(tr("View purchase order report"));
        ga->setFactory(ACC_ActionPurchaseOrderReport::factory);
    }
    return ga;
}

RB_Action* ACC_ActionPurchaseOrderReport::factory() {
    RB_Action* a = new ACC_ActionPurchaseOrderReport();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionPurchaseOrderReport::trigger() {
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
            ACC_DialogFactory::WidgetPurchaseOrderReport);

    // set purchase order after init(), sales order setting is default
    ACC_OrderReportWidget* report = dynamic_cast<ACC_OrderReportWidget*>(mdiWin->getWidget());
    report->setPurchaseOrder();

    mdiWin->show();
    mdiWin->raise();

    // show toolbars
    QToolBar* toolBar = ACC_DIALOGFACTORY->getMainWindow()->findChild<QToolBar*>("toolbar_edit");
    if (toolBar) {
        toolBar->show();
    }
}
