/*****************************************************************
 * $Id: peng_actionclose.h 2185 2014-10-09 15:09:35Z rutger $
 * Created: Oct 8, 2014 8:14:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_ACTIONCLOSE_H
#define PENG_ACTIONCLOSE_H

#include "rb_action.h"


/**
 * Action closes the perspective
 */
class PENG_ActionClose : public RB_Action {

    Q_OBJECT

public:
    PENG_ActionClose();
    virtual ~PENG_ActionClose() {}

    static RB_String getName() {
        return "Close perspective";
    }

    virtual RB_String name() {
        return PENG_ActionClose::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PENG_ACTIONCLOSE_H
