/*****************************************************************
 * $Id: db_actionsystemselectproject.cpp 1113 2010-03-15 14:48:33Z rutger $
 * Created: Aug 6, 2010 21:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontestsystemselectproject.h"

#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "rb_database.h"
#include "rb_dialogwindow.h"
#include "rb_mainwindow.h"
#include "rb_modelfactory.h"


DB_ActionTestSystemSelectProject::DB_ActionTestSystemSelectProject()
        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionTestSystemSelectProject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionTestSystemSelectProject::getName(),
                                  "dialog",
                                  tr("&Select project"));
//        ga->setCommand("da, database");
//        ga->setKeycode("daa");
//        ga->setStatusTip(tr("Connect to database"));
//        ga->setShortcut("Ctrl+B");
//        ga->setToolTip("Connect to database");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionTestSystemSelectProject::factory);
    }
    return ga;
}

RB_Action* DB_ActionTestSystemSelectProject::factory() {
    RB_Action* a = new DB_ActionTestSystemSelectProject();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionTestSystemSelectProject::trigger() {
    if (RB_DATABASE->database().isOpen()) {

        RB_DialogWindow* dlg = DB_DIALOGFACTORY->getDialogWindow(
                    DB_DialogFactory::WidgetTestSelectProject);

        if (dlg->exec() == QDialog::Accepted) {
//            QModelIndex index = dlg->getCurrentModelIndex();
//            if (!index.isValid()) return;

//            const RB_MmProxy* m = dynamic_cast<const RB_MmProxy*>(index.model());
//            RB_String id = m->hiddenData(m->index(index.row(), 0, index.parent()),
//                                         Qt::DisplayRole).toString();


            RB_ObjectBase* obj = dlg->getCurrentObject();
            if (!obj) {
                delete dlg;
                return;
            }

            RB_String id = obj->getId();

            // Set root id
            DB_MODELFACTORY->setRootId(id);
            // Get the main project root object for this application
            RB_ObjectBase* root = DB_MODELFACTORY->getRoot();
            // Read the object from the database for further use
            root->dbRead(DB_MODELFACTORY->getDatabase(), RB2::ResolveNone);
        } else  {
            DB_MODELFACTORY->setRootId("");
        }
        delete dlg;
    } else {
        DB_DIALOGFACTORY->requestWarningDialog(tr("Not connected to database."));
    }
}
