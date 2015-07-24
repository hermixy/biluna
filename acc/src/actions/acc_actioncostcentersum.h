/*****************************************************************
 * $Id: acc_actioncostcentersum.h 1692 2012-07-09 20:15:01Z rutger $
 * Created: Jul 9, 2012 8:44:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONCOSTCENTERSUM_H
#define ACC_ACTIONCOSTCENTERSUM_H

#include "rb_action.h"


/**
 * Action opens trial balance view dialog
 */
class ACC_ActionCostCenterSum : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionCostCenterSum();
    virtual ~ACC_ActionCostCenterSum() {}

    static RB_String getName() {
        return "View cost center summary";
    }

    virtual RB_String name() {
        return ACC_ActionCostCenterSum::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONCOSTCENTERSUM_H
