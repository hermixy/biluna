/*****************************************************************
 * $Id: acc_actionarea.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Feb 21, 2010 11:14:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONAREA_H
#define ACC_ACTIONAREA_H

#include "rb_action.h"


/**
 * Action opens sales area edit dialog
 */
class ACC_ActionArea : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionArea();
    virtual ~ACC_ActionArea() {}

    static RB_String getName() {
        return "Sales area";
    }

    virtual RB_String name() {
        return ACC_ActionArea::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONAREA_H
