/*****************************************************************
 * $Id: db_actiontestdbobject.cpp 1092 2010-02-10 09:52:33Z rutger $
 * Created: Dec 15, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontestdbobject.h"
#include "db_modelfactory.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "rb_mdiwindow.h"
#include "rb_objectcontainer.h"


DB_ActionTestDbObject::DB_ActionTestDbObject()
        : RB_Action() {
//    mTextWidget = tw;
}


RB_GuiAction* DB_ActionTestDbObject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionTestDbObject::getName(),
                                  "fileclose",
                                  tr("&DB Object"));
        ga->setCommand("db object, db");
        ga->setKeycode("db");
        ga->setStatusTip(tr("Test DB object"));
//        ga->setShortcut("Ctrl+O");
//        ga->setToolTip("Test DB object");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionTestDbObject::factory);
    }
    return ga;
}

RB_Action* DB_ActionTestDbObject::factory() {
    RB_Action* a = new DB_ActionTestDbObject();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionTestDbObject::trigger() {
    // No preliminary checks required

    // Prepare model(s)
//    RB_MmObjectInterface* model = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelTest);
//    model->setRoot(DB_MODELFACTORY->getRootId());

    // Create widget
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMdiWindow(
            DB_DialogFactory::WidgetTestDbObject);
//    DB_TestDbObjectWidget* tw = dynamic_cast<DB_TestDbObjectWidget*>(mdiWin->getWidget());
//    tw->init(DB_ModelFactory::ModelTest, DB_ModelFactory::ModelTestChild);
    mdiWin->show();
}

