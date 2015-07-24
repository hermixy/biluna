/*****************************************************************
 * $Id: scan_actionreviewchart.cpp 1468 2011-10-12 23:23:42Z rutger $
 * Created: Oct 10, 2011 8:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_actionreviewchart.h"

#include "scan_dialogfactory.h"
#include "scan_modelfactory.h"
#include "rb_mdiwindow.h"


SCAN_ActionReviewChart::SCAN_ActionReviewChart()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SCAN_ActionReviewChart::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SCAN_ActionReviewChart::getName(),
                              QIcon(":/images/icons/chart_pie_edit.png"),
                              tr("Review chart types"));
        ga->setStatusTip(tr("Review chart types"));
        ga->setFactory(SCAN_ActionReviewChart::factory);
    }
    return ga;
}

RB_Action* SCAN_ActionReviewChart::factory() {
    RB_Action* a = new SCAN_ActionReviewChart();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SCAN_ActionReviewChart::trigger() {
    // Check required settings
    if (SCAN_MODELFACTORY->getRootId() == ""
            || !SCAN_MODELFACTORY->getDatabase().isOpen()) {
        SCAN_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and than select project."));
        return;
    }

    // Create widget
    RB_MdiWindow* mdiWin = SCAN_DIALOGFACTORY->getMdiWindow(
                SCAN_DialogFactory::WidgetReviewChart);
    mdiWin->show();
    mdiWin->raise();
}
