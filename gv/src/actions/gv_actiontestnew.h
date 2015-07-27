/*****************************************************************
 * $Id: gv_actiontestnew.h 1452 2011-09-10 08:19:50Z rutger $
 * Created: Oct 2, 2008 4:35:20 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionfilenew.h
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONTESTNEW_H
#define GV_ACTIONTESTNEW_H

#include "rb_action.h"


/**
 * This action class create new test RGraphicsView MDI child
 */
class GV_ActionTestNew : public RB_Action {
	
    Q_OBJECT
    
public:
    GV_ActionTestNew();
    ~GV_ActionTestNew() {}

    static RB_String getName() {
        return "New Drawing";
    }

    virtual RB_String name() {
        return GV_ActionTestNew::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
