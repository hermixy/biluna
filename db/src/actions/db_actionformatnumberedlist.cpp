/*****************************************************************
 * $Id: db_actionformatnumberedlist.cpp 1406 2011-05-04 11:28:22Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionformatnumberedlist.h"

#include "db_dialogfactory.h"
#include "rb_mdiwindow.h"
#include "rb_widget.h"

DB_ActionFormatNumberedList::DB_ActionFormatNumberedList() : RB_Action() {}


RB_GuiAction* DB_ActionFormatNumberedList::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionFormatNumberedList::getName(),
                                  QIcon(":/images/icons/text_list_numbers.png"),
                                  tr("Format &Numbered List"));
        ga->setStatusTip(tr("Format Numbered List"));
        ga->setCheckable(true);

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        ga->setFactory(DB_ActionFormatNumberedList::factory);
    }
    return ga;
}

RB_Action* DB_ActionFormatNumberedList::factory() {
    RB_Action* a = new DB_ActionFormatNumberedList();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionFormatNumberedList::trigger() {
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
        wdgt->formatNumberedList();
    }
}
