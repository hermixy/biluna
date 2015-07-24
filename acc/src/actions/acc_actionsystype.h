/*****************************************************************
 * $Id: acc_actionsystype.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Feb 8, 2012 8:24:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONSYSTYPE_H
#define ACC_ACTIONSYSTYPE_H

#include "rb_action.h"


/**
 * Action opens system type number edit dialog
 */
class ACC_ActionSysType : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionSysType();
    virtual ~ACC_ActionSysType() {}

    static RB_String getName() {
        return "System type numbers";
    }

    virtual RB_String name() {
        return ACC_ActionSysType::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONSYSTYPE_H
