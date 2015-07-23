/*****************************************************************
 * $Id: db_actionformatfontsize.cpp 1184 2010-06-08 21:59:15Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionformatfontsize.h"

#include "db_dialogfactory.h"
#include "db_htmleditorwidget.h"
#include "rb_mdiwindow.h"
#include "rb_widget.h"

DB_ActionFormatFontSize::DB_ActionFormatFontSize() : RB_Action() {}


RB_GuiAction* DB_ActionFormatFontSize::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionFormatFontSize::getName(),
                                  QIcon(":/images/icons/text_smallcaps.png"),
                                  tr("Font Si&ze"));
        ga->setStatusTip(tr("Font Size"));

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        ga->setFactory(DB_ActionFormatFontSize::factory);
    }
    return ga;
}

RB_Action* DB_ActionFormatFontSize::factory() {
    RB_Action* a = new DB_ActionFormatFontSize();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionFormatFontSize::trigger() {
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
        wdgt->formatFontSize();
    }
}
