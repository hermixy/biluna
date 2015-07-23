/*****************************************************************
 * $Id: db_actionformataligncenter.cpp 1394 2011-04-06 22:21:12Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionformataligncenter.h"

#include "db_dialogfactory.h"
#include "db_htmleditorwidget.h"
#include "rb_mdiwindow.h"
#include "rb_widget.h"

DB_ActionFormatAlignCenter::DB_ActionFormatAlignCenter() : RB_Action() {}


RB_GuiAction* DB_ActionFormatAlignCenter::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionFormatAlignCenter::getName(),
                                  QIcon(":/images/icons/text_align_center.png"),
                                  tr("Align &Center"));
        ga->setStatusTip(tr("Align Center"));

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        ga->setFactory(DB_ActionFormatAlignCenter::factory);
    }
    return ga;
}

RB_Action* DB_ActionFormatAlignCenter::factory() {
    RB_Action* a = new DB_ActionFormatAlignCenter();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionFormatAlignCenter::trigger() {
    // No preliminary checks required

    // No models to prepare

    // Get active widget
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getActiveMdiWindow();
    if (!mdiWin) return;
    RB_Widget* wdgt = mdiWin->getWidget();
    if (!wdgt) {
        wdgt = mWidget;
    }

    if (wdgt) {
        // wdgt->init(); not required
        wdgt->formatAlignCenter();
    }
}
