/*****************************************************************
 * $Id: db_actionsystemstylecolorpicker.cpp 2195 2014-11-10 12:26:20Z rutger $
 * Created: Oct 29, 2014 8:14:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystemstylecolorpicker.h"

#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "rb_mdiwindow.h"


DB_ActionSystemStyleColorPicker::DB_ActionSystemStyleColorPicker()
                        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionSystemStyleColorPicker::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionSystemStyleColorPicker::getName(),
                              //QIcon(":/images/icons/star.png"),
                              tr("&Palette color picker"));
        ga->setStatusTip(tr("GUI Palette color picker"));
        ga->setFactory(DB_ActionSystemStyleColorPicker::factory);
    }
    return ga;
}

RB_Action* DB_ActionSystemStyleColorPicker::factory() {
    RB_Action* a = new DB_ActionSystemStyleColorPicker();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionSystemStyleColorPicker::trigger() {
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMdiWindow(
            DB_DialogFactory::WidgetPaletteColorPicker);
    if (!mdiWin) return;
    mdiWin->show();
    mdiWin->raise();
}
