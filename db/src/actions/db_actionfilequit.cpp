/*****************************************************************
 * $Id: db_actionfilequit.cpp 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionfilequit.h"



DB_ActionFileQuit::DB_ActionFileQuit() : RB_Action() {

}


RB_GuiAction* DB_ActionFileQuit::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionFileQuit::getName(),
                              QIcon(":/images/icons/cross.png"),
                              tr("&Quit"));
        ga->setShortcut("Ctrl+Q");
        ga->setStatusTip(tr("Quits the application"));
    }                                  
    return ga;
}
