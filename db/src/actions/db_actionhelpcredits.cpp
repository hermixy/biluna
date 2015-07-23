/*****************************************************************
 * $Id: db_actionhelpcredits.cpp 1378 2011-03-03 22:15:20Z rutger $
 * Created: Dec 22, 2009 3:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionhelpcredits.h"


DB_ActionHelpCredits::DB_ActionHelpCredits() : RB_Action() {

}


RB_GuiAction* DB_ActionHelpCredits::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionHelpCredits::getName(),
                              tr("&Credits"));
        ga->setStatusTip(tr("Credits"));
    }                                  
    return ga;
}


