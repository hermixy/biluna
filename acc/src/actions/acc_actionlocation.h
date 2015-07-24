/*****************************************************************
 * $Id: acc_actionlocation.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Feb 18, 2010 9:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONLOCATION_H
#define ACC_ACTIONLOCATION_H

#include "rb_action.h"


/**
 * Action opens stock location edit dialog
 */
class ACC_ActionLocation : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionLocation();
    virtual ~ACC_ActionLocation() {}

    static RB_String getName() {
        return "Stock location";
    }

    virtual RB_String name() {
        return ACC_ActionLocation::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONLOCATION_H
