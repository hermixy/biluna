/*****************************************************************
 * $Id: db_actionfilenewhtmlview.cpp 1468 2011-10-12 23:23:42Z rutger $
 * Created: Jan 13, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionfilenewhtmlview.h"

#include "db_dialogfactory.h"
//#include "db_htmlviewwidget.h"
#include "db_internetbrowserfactory.h"
#include "db_internetbrowserwidget.h"
#include "rb_mdiwindow.h"

DB_ActionFileNewHtmlView::DB_ActionFileNewHtmlView() : RB_Action() {}


RB_GuiAction* DB_ActionFileNewHtmlView::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(DB_ActionFileNewHtmlView::getName(),
                                  QIcon(":/images/icons/world_link.png"),
                                  tr("Internet &Browser"));
        action->setStatusTip(tr("Creates a new Internet browser"));
        action->setFactory(DB_ActionFileNewHtmlView::factory);
    }                         
    return action;
}

RB_Action* DB_ActionFileNewHtmlView::factory() {
    RB_Action* a = new DB_ActionFileNewHtmlView();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionFileNewHtmlView::trigger() {
    try {
/*
    // No preliminary checks required

    // No model preparation

    // Create widget
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMdiWindow(
            DB_DialogFactory::WidgetInternet);
    if (!mdiWin) return;
//    DB_HtmlViewWidget* wdgt = dynamic_cast<DB_HtmlViewWidget*>(mdiWin->getWidget());
//    tw->init(DB_ModelFactory::ModelTest, DB_ModelFactory::ModelTestChild);

    DB_InternetBrowserWidget* wdgt = dynamic_cast<DB_InternetBrowserWidget*>(mdiWin->getWidget());

    if (!wdgt) {
        // widget set from outside
        wdgt = dynamic_cast<DB_InternetBrowserWidget*>(mWidget);
    }

    if (wdgt) {
        // wdgt->init(); not required
        wdgt->loadPage("http://www.biluna.com");
    }
    mdiWin->show();
 */
        DB_InternetBrowserWidget* wdgt = DB_INTERNETBROWSERFACTORY->newBrowserWidget();
        wdgt->loadPage("https://www.google.com");
    } catch(std::exception& e) {
        DB_DIALOGFACTORY->requestWarningDialog(e.what());
    } catch(...) {
        DB_DIALOGFACTORY->requestWarningDialog(
                    "Error <unkown> in DB_InternetBrowserWidget.trigger()");
    }
}

