/*****************************************************************
 * $Id: acc_actionholdreason.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Feb 18, 2010 8:14:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONHOLDREASON_H
#define ACC_ACTIONHOLDREASON_H

#include "rb_action.h"


/**
 * Action opens customer credit status or hold reasons edit dialog
 */
class ACC_ActionHoldReason : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionHoldReason();
    virtual ~ACC_ActionHoldReason() {}

    static RB_String getName() {
        return "Hold reasons";
    }

    virtual RB_String name() {
        return ACC_ActionHoldReason::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONHOLDREASON_H
