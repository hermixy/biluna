/*****************************************************************
 * $Id: db_actionstyleheading1.cpp 1092 2010-02-10 09:52:33Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionstyleheading1.h"

#include "db_dialogfactory.h"
#include "db_htmleditorwidget.h"
#include "rb_mdiwindow.h"
#include "rb_widget.h"

DB_ActionStyleHeading1::DB_ActionStyleHeading1() : RB_Action() {}


RB_GuiAction* DB_ActionStyleHeading1::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionStyleHeading1::getName(),
                                  QIcon(":/images/icons/text_heading_1.png"),
                                  tr("Heading &1"));
        ga->setStatusTip(tr("Heading 1"));

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        ga->setFactory(DB_ActionStyleHeading1::factory);
    }
    return ga;
}

RB_Action* DB_ActionStyleHeading1::factory() {
    RB_Action* a = new DB_ActionStyleHeading1();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionStyleHeading1::trigger() {
    // No preliminary checks required

    // No models to prepare

    // Get active widget
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getActiveMdiWindow();
    if (!mdiWin) return;
    DB_HtmlEditorWidget* wdgt = dynamic_cast<DB_HtmlEditorWidget*>(mdiWin->getWidget());
    if (!wdgt) {
        wdgt = dynamic_cast<DB_HtmlEditorWidget*>(mWidget);
    }

    if (wdgt) {
        // wdgt->init(); not required
        wdgt->styleHeading1();
    }
}
