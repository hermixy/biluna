/*****************************************************************
 * $Id: gv_actionfilenew.cpp 1148 2010-05-08 11:53:42Z rutger $
 * Created: Oct 2, 2008 4:35:57 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Base on rs_actionfilenew.cpp
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionfilenew.h"

#include "gv_dialogfactory.h"
#include "gv_modelfactory.h"
#include "rb_mdiwindow.h"

/**
 * Constructor
 */
GV_ActionFileNew::GV_ActionFileNew() : RB_Action() {

}


RB_GuiAction* GV_ActionFileNew::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(GV_ActionFileNew::getName(),
                        QIcon(":/images/icons/page_white.png"),
                        tr("&New Drawing"));
        ga->setStatusTip(tr("Creates a new drawing in this project"));
//        ga->setShortcut("Ctrl+N");
        ga->setFactory(GV_ActionFileNew::factory);
    }                         
    return ga;
}

RB_Action* GV_ActionFileNew::factory() {
    RB_Action* a = new GV_ActionFileNew();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void GV_ActionFileNew::trigger() {
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
            GV_DialogFactory::WidgetDrawing);



    mdiWin->show();
}

