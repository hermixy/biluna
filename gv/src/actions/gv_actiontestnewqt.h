/*****************************************************************
 * $Id: gv_actiontestnewqt.h 1452 2011-09-10 08:19:50Z rutger $
 * Created: Oct 2, 2008 4:35:20 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionfilenew.h
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONTESTNEWQT_H
#define GV_ACTIONTESTNEWQT_H

#include "rb_action.h"


/**
 * This action class create new test RGraphicsViewQt MDI child
 */
class GV_ActionTestNewQt : public RB_Action {
	
    Q_OBJECT
    
public:
    GV_ActionTestNewQt();
    ~GV_ActionTestNewQt() {}

    static RB_String getName() {
        return "New Drawing";
    }

    virtual RB_String name() {
        return GV_ActionTestNewQt::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
