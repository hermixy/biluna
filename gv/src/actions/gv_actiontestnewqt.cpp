/*****************************************************************
 * $Id: gv_actiontestnewqt.cpp 1452 2011-09-10 08:19:50Z rutger $
 * Created: Oct 2, 2008 4:35:57 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Base on rs_actionfilenew.cpp
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actiontestnewqt.h"

#include "gv_dialogfactory.h"
#include "gv_modelfactory.h"
#include "rb_mdiwindow.h"

/**
 * Constructor
 */
GV_ActionTestNewQt::GV_ActionTestNewQt() : RB_Action() {

}


RB_GuiAction* GV_ActionTestNewQt::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(GV_ActionTestNewQt::getName(),
                        QIcon(":/images/icons/page_white.png"),
                        tr("&New Drawing Qt"));
        ga->setStatusTip(tr("Creates a new drawing in this project"));
//        ga->setShortcut("Ctrl+N");
        ga->setFactory(GV_ActionTestNewQt::factory);
    }                         
    return ga;
}

RB_Action* GV_ActionTestNewQt::factory() {
    RB_Action* a = new GV_ActionTestNewQt();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void GV_ActionTestNewQt::trigger() {
    // Check required settings
//    if (GV_MODELFACTORY->getRootId() == ""
//            || !GV_MODELFACTORY->getDatabase().isOpen()) {
//        GV_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
//                                                   "Connect first to database\n"
//                                                   "and than select project."));
//        return;
//    }

    // Create widget and corresponding dockwidget if applicable
    RB_MdiWindow* mdiWin = GV_DIALOGFACTORY->getMdiWindow(
            GV_DialogFactory::WidgetGraphicsViewQt);



    mdiWin->show();
}

