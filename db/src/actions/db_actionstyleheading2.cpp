/*****************************************************************
 * $Id: db_actionstyleheading2.cpp 1393 2011-03-31 21:36:46Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionstyleheading2.h"

#include "db_dialogfactory.h"
#include "db_htmleditorwidget.h"
#include "rb_mdiwindow.h"
#include "rb_widget.h"

DB_ActionStyleHeading2::DB_ActionStyleHeading2() : RB_Action() {}


RB_GuiAction* DB_ActionStyleHeading2::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionStyleHeading2::getName(),
                                  QIcon(":/images/icons/text_heading_2.png"),
                                  tr("Heading &2"));
        ga->setStatusTip(tr("Heading 2"));

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        ga->setFactory(DB_ActionStyleHeading2::factory);
    }
    return ga;
}

RB_Action* DB_ActionStyleHeading2::factory() {
    RB_Action* a = new DB_ActionStyleHeading2();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionStyleHeading2::trigger() {
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
        wdgt->styleHeading2();
    }
}
