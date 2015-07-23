/*****************************************************************
 * $Id: db_actionsystemcalendar.cpp 1927 2013-04-22 15:22:10Z rutger $
 * Created: Apr 22, 2013 4:14:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystemcalendar.h"

#include "db_dialogfactory.h"
#include "rb_mdiwindow.h"


DB_ActionSystemCalendar::DB_ActionSystemCalendar()
        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionSystemCalendar::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionSystemCalendar::getName(),
                              QIcon(":/images/icons/calendar.png"),
                              tr("C&alendar"));
        ga->setStatusTip(tr("Calendar"));
        ga->setFactory(DB_ActionSystemCalendar::factory);
    }
    return ga;
}

RB_Action* DB_ActionSystemCalendar::factory() {
    RB_Action* a = new DB_ActionSystemCalendar();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionSystemCalendar::trigger() {
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMdiWindow(
            DB_DialogFactory::WidgetCalendar);

    mdiWin->show();
    mdiWin->raise();
}
