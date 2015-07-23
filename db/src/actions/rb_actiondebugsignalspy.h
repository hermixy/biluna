/*****************************************************************
 * $Id: rb_actiondebugsignalspy.h 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 16, 2009 12:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_ACTIONDEBUGSIGNALSPY_H
#define RB_ACTIONDEBUGSIGNALSPY_H

#include "rb_action.h"


/**
 * Debug signal and slot action class
 */
class RB_ActionDebugSignalSpy : public RB_Action {

    Q_OBJECT

public:
    RB_ActionDebugSignalSpy();
    virtual ~RB_ActionDebugSignalSpy() {}

    static RB_String getName() {
        return "Debug signal slots";
    }

    virtual RB_String name() {
        return RB_ActionDebugSignalSpy::getName();
    }

    static RB_GuiAction* createGuiAction();
};

#endif // RB_ACTIONDEBUGSIGNALSPY_H

