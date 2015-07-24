/*****************************************************************
 * $Id: acc_actioncostgroup.h 1680 2012-06-23 15:08:20Z rutger $
 * Created: Jun 21, 2012 14:00:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONCOSTGROUP_H
#define ACC_ACTIONCOSTGROUP_H

#include "rb_action.h"


/**
 * Action opens cost center group edit dialog
 */
class ACC_ActionCostGroup : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionCostGroup();
    virtual ~ACC_ActionCostGroup() {}

    static RB_String getName() {
        return "Cost Center Group";
    }

    virtual RB_String name() {
        return ACC_ActionCostGroup::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONCOSTGROUP_H
