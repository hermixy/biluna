/*****************************************************************
 * $Id: gv_actionsystemselectproject.cpp 2199 2014-12-08 21:41:10Z rutger $
 * Created: Apr 19, 2010 21:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna GV project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionsystemselectproject.h"

#include "gv_dialogfactory.h"
#include "gv_projectdialog.h"
#include "rb_database.h"
#include "rb_mainwindow.h"
#include "rb_modelfactory.h"


GV_ActionSystemSelectProject::GV_ActionSystemSelectProject()
        : RB_Action() {
    // nothing
}


RB_GuiAction* GV_ActionSystemSelectProject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(GV_ActionSystemSelectProject::getName(),
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
        ga->setFactory(GV_ActionSystemSelectProject::factory);
    }
    return ga;
}

RB_Action* GV_ActionSystemSelectProject::factory() {
    RB_Action* a = new GV_ActionSystemSelectProject();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void GV_ActionSystemSelectProject::trigger() {
    if (RB_Database::database().isOpen()) {
        RB_Dialog* dlg = GV_DIALOGFACTORY->getDialog(GV_DialogFactory::DialogProject);
        if (dlg->exec() == QDialog::Accepted) {
            RB_ObjectBase* obj = dlg->getCurrentObject();

            if (!obj) {
                RB_DEBUG->error("GV_ActionSystemSelectProject::trigger() "
                                "selected object is NULL ERROR");
                delete dlg;
                return;
            }

            RB_String id = obj->getId();
            GV_MODELFACTORY->setRootId(id);
            RB_ObjectBase* root = GV_MODELFACTORY->getRoot();

            if (!root) {
                RB_DEBUG->print(RB_Debug::D_ERROR,
                                "GV_ActionSystemSelectProject::trigger() "
                                "root is NULL ERROR");
            } else {
                root->dbRead(GV_MODELFACTORY->getDatabase(), RB2::ResolveNone);
            }
        } else  {
            GV_MODELFACTORY->setRootId("");
        }
        delete dlg;
    } else {
        GV_DIALOGFACTORY->requestWarningDialog(tr("Not connected to database."));
    }
}
