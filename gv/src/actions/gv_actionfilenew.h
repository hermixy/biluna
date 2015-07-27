/*****************************************************************
 * $Id: gv_actionfilenew.h 1148 2010-05-08 11:53:42Z rutger $
 * Created: Oct 2, 2008 4:35:20 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionfilenew.h
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONFILENEW_H
#define GV_ACTIONFILENEW_H

#include "rb_action.h"


/**
 * This action class can handle user events to create new blank documents.
 * Inherits from RB_Action and not from GV_ActionBase because no eventhandling
 * at drawing level required.
 */
class GV_ActionFileNew : public RB_Action {
	
    Q_OBJECT
    
public:
    GV_ActionFileNew();
    ~GV_ActionFileNew() {}

    static RB_String getName() {
        return "New Drawing";
    }

    virtual RB_String name() {
        return GV_ActionFileNew::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
