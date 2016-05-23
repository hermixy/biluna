/*****************************************************************
 * $Id: db_actiontestcleartext.cpp 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontestcleartext.h"
#include "db_testdialogfactory.h"
#include "db_testtextwidget.h"
#include "rb_mdiwindow.h"

DB_ActionTestClearText::DB_ActionTestClearText()
        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionTestClearText::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionTestClearText::getName(),
                                  "fileclose",
                                  tr("&Clear Text"));
        ga->setCommand("ctd,ct");
        ga->setKeycode("ctd");
        ga->setStatusTip(tr("Clear text document"));
        ga->setShortcut("Ctrl+C");
        ga->setToolTip("Clear text document");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionTestClearText::factory);
    }                                  
    return ga;
}


RB_Action* DB_ActionTestClearText::factory() {

    RB_Action* a = new DB_ActionTestClearText();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;


    // Alternatively:

//    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
//    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
//
//    if (container!=NULL && graphicView!=NULL) {
//        RS_ActionInterface* a = new RS_ActionDrawLine(*container, *graphicView);
//        graphicView->setCurrentAction(a);
//        return a;
//    }
//    return NULL;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionTestClearText::trigger() {
    RB_MdiWindow* mdiWin = DB_TESTDIALOGFACTORY->getMdiWindow(
            DB_TestDialogFactory::WidgetTestText);
    DB_TestTextWidget* tw = dynamic_cast<DB_TestTextWidget*>(mdiWin->getWidget());

    tw->slotClearText();
}
