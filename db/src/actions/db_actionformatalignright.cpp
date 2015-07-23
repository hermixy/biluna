/*****************************************************************
 * $Id: db_actionformatalignright.cpp 1184 2010-06-08 21:59:15Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionformatalignright.h"

#include "db_dialogfactory.h"
#include "db_htmleditorwidget.h"
#include "rb_mdiwindow.h"
#include "rb_widget.h"

DB_ActionFormatAlignRight::DB_ActionFormatAlignRight() : RB_Action() {}


RB_GuiAction* DB_ActionFormatAlignRight::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionFormatAlignRight::getName(),
                                  QIcon(":/images/icons/text_align_right.png"),
                                  tr("Align &Right"));
        ga->setStatusTip(tr("Align right"));

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        ga->setFactory(DB_ActionFormatAlignRight::factory);
    }
    return ga;
}

RB_Action* DB_ActionFormatAlignRight::factory() {
    RB_Action* a = new DB_ActionFormatAlignRight();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionFormatAlignRight::trigger() {
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
        wdgt->formatAlignRight();
    }
}
