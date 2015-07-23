/*****************************************************************
 * $Id: db_actionformatbackgroundcolor.cpp 1406 2011-05-04 11:28:22Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionformatbackgroundcolor.h"

#include "db_dialogfactory.h"
#include "rb_mdiwindow.h"
#include "rb_widget.h"

DB_ActionFormatBackgroundColor::DB_ActionFormatBackgroundColor() : RB_Action() {}


RB_GuiAction* DB_ActionFormatBackgroundColor::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionFormatBackgroundColor::getName(),
                                  QIcon(":/images/icons/text_background_color.png"),
                                  tr("Bac&kground Color ..."));
        ga->setStatusTip(tr("Background Color"));

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        ga->setFactory(DB_ActionFormatBackgroundColor::factory);
    }
    return ga;
}

RB_Action* DB_ActionFormatBackgroundColor::factory() {
    RB_Action* a = new DB_ActionFormatBackgroundColor();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionFormatBackgroundColor::trigger() {
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
        wdgt->formatBackgroundColor();
    }
}
