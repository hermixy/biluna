/*****************************************************************
 * $Id: gv_actiondrawinglist.cpp 1341 2010-12-31 11:54:04Z rutger $
 * Created: Apr 21, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actiondrawinglist.h"

#include "gv_dialogfactory.h"
#include "gv_modelfactory.h"
#include "rb_dockwidget.h"
#include "rb_mdiwindow.h"


GV_ActionDrawingList::GV_ActionDrawingList()
        : RB_Action() {
    // nothing
}


RB_GuiAction* GV_ActionDrawingList::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(GV_ActionDrawingList::getName(),
                              QIcon(":/images/icons/page_white_stack.png"),
                              tr("&Drawing list"));
        ga->setStatusTip(tr("List of drawings"));
        ga->setFactory(GV_ActionDrawingList::factory);
    }
    return ga;
}

RB_Action* GV_ActionDrawingList::factory() {
    RB_Action* a = new GV_ActionDrawingList();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void GV_ActionDrawingList::trigger() {
    // Check required settings
    if (GV_MODELFACTORY->getRootId() == ""
            || !GV_MODELFACTORY->getDatabase().isOpen()) {
        GV_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and than select project."));
        return;
    }

    // Create widget
    RB_DockWidget* dwgt = GV_DIALOGFACTORY->getDockWidget(
            GV_DialogFactory::WidgetDrawingList,
            "dockwidget_drawinglist",
            Qt::LeftDockWidgetArea);
    dwgt->show();
}
