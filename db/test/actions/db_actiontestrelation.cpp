/*****************************************************************
 * $Id: db_actiontestrelation.cpp 1771 2012-09-06 21:13:20Z rutger $
 * Created: Aug 9, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontestrelation.h"

#include "db_modelfactory.h"
#include "db_testdialogfactory.h"
#include "db_testmodelfactory.h"
#include "db_testobjectfactory.h"
#include "db_testtablewidget.h"
#include "rb_mdiwindow.h"


DB_ActionTestRelation::DB_ActionTestRelation()
        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionTestRelation::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionTestRelation::getName(),
                                  "fileclose",
                                  tr("&Relation Database Model"));
        ga->setStatusTip(tr("Test relation database table model"));
        ga->setFactory(DB_ActionTestRelation::factory);
    }
    return ga;
}

RB_Action* DB_ActionTestRelation::factory() {
    RB_Action* a = new DB_ActionTestRelation();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionTestRelation::trigger() {
    // No preliminary checks required

    // No model(s) to prepare

    // Create dialog
    RB_Dialog* dlg = DB_TESTDIALOGFACTORY->getDialog(DB_TestDialogFactory::DialogTestRelation);
    dlg->exec();
    delete dlg;
}
