/*****************************************************************
 * $Id: sail_actionclose.h 2185 2014-10-09 15:09:35Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef MRP_ACTIONCLOSE_H
#define MRP_ACTIONCLOSE_H

#include "rb_action.h"


/**
 * Action closes the perspective
 */
class MRP_ActionClose : public RB_Action {

    Q_OBJECT

public:
    MRP_ActionClose();
    virtual ~MRP_ActionClose() {}

    static RB_String getName() {
        return "Close perspective";
    }

    virtual RB_String name() {
        return MRP_ActionClose::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // MRP_ACTIONCLOSE_H
