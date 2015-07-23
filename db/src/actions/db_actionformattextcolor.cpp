/*****************************************************************
 * $Id: db_actionformattextcolor.cpp 1197 2010-06-13 12:04:43Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionformattextcolor.h"

#include "db_dialogfactory.h"
#include "db_htmleditorwidget.h"
#include "rb_mdiwindow.h"
#include "rb_widget.h"

DB_ActionFormatTextColor::DB_ActionFormatTextColor() : RB_Action() {}


RB_GuiAction* DB_ActionFormatTextColor::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionFormatTextColor::getName(),
                                  QIcon(":/images/icons/text_color.png"),
                                  tr("Text &Color"));
        ga->setStatusTip(tr("Text Color"));

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        ga->setFactory(DB_ActionFormatTextColor::factory);
    }
    return ga;
}

RB_Action* DB_ActionFormatTextColor::factory() {
    RB_Action* a = new DB_ActionFormatTextColor();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionFormatTextColor::trigger() {
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
        wdgt->formatTextColor();
    }
}
