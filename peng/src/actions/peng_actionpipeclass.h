/*****************************************************************
 * $Id: peng_actionfluid.h 2146 2014-05-23 20:27:06Z rutger $
 * Created: June 5, 2014 9:14:42 PM - rutger
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_ACTIONPIPECLASS_H
#define PENG_ACTIONPIPECLASS_H

#include "rb_action.h"


/**
 * Action opens pipe class edit/selection dialog
 */
class PENG_ActionPipeClass : public RB_Action {

    Q_OBJECT

public:
    PENG_ActionPipeClass();
    virtual ~PENG_ActionPipeClass() {}

    static RB_String getName() {
        return "Pipe Class";
    }

    virtual RB_String name() {
        return PENG_ActionPipeClass::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PENG_ACTIONPIPECLASS_H
