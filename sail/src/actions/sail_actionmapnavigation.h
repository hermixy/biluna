/*****************************************************************
 * $Id: sail_actionmapnavigation.h 1723 2012-07-27 14:42:24Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_ACTIONMAPNAVIGATION_H
#define SAIL_ACTIONMAPNAVIGATION_H

#include "rb_action.h"


/**
 * Action opens map navigation window
 */
class SAIL_ActionMapNavigation : public RB_Action {

    Q_OBJECT

public:
    SAIL_ActionMapNavigation();
    virtual ~SAIL_ActionMapNavigation() {}

    static RB_String getName() {
        return "Map Navigation";
    }

    virtual RB_String name() {
        return SAIL_ActionMapNavigation::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SAIL_ACTIONMAPNAVIGATION_H
