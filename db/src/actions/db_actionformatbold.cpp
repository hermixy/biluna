/*****************************************************************
 * $Id: db_actionformatbold.cpp 1184 2010-06-08 21:59:15Z rutger $
 * Created: Jan 9, 2009 1:07:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionformatbold.h"

#include "db_dialogfactory.h"
#include "db_htmleditorwidget.h"
#include "rb_mdiwindow.h"
#include "rb_widget.h"

DB_ActionFormatBold::DB_ActionFormatBold() : RB_Action() {}


RB_GuiAction* DB_ActionFormatBold::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionFormatBold::getName(),
                                  QIcon(":/images/icons/text_bold.png"),
                                  tr("&Bold"));
        ga->setStatusTip(tr("Format Bold"));
        ga->setShortcut("Ctrl+B");
        ga->setCheckable(true);

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionFormatBold::factory);
    }
    return ga;
}

RB_Action* DB_ActionFormatBold::factory() {
    RB_Action* a = new DB_ActionFormatBold();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionFormatBold::trigger() {
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
        wdgt->formatBold();
    }
}

///**
// * Update action state to checked or enabled
// */
//void DB_ActionFormatBold::slotUpdateAction() {
//    DB_HtmlEditorWidget* wdgt = dynamic_cast<DB_HtmlEditorWidget*>(mWidget);
//    if (!wdgt) return;
//    wdgt->pageActionChecked(QWebPage::ToggleBold);
//}
