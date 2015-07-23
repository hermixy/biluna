/*****************************************************************
 * $Id: db_actionhelpindex.cpp 1378 2011-03-03 22:15:20Z rutger $
 * Created: Nov 26, 2009 3:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionhelpindex.h"

DB_ActionHelpIndex::DB_ActionHelpIndex() : RB_Action() {

}


RB_GuiAction* DB_ActionHelpIndex::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionHelpIndex::getName(),
                              tr("&Index"));
        ga->setStatusTip(tr("Help index"));
    }
    return ga;
}
