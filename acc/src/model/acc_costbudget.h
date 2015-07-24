/*****************************************************************
 * $Id: acc_costbudget.h 1677 2012-06-20 20:32:29Z rutger $
 * Created: Jun 20, 2012 6:04:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_COSTBUDGET_H
#define ACC_COSTBUDGET_H

#include "rb_objectcontainer.h"

/**
 * Class for cost center budget per period (year or month)
 */
class ACC_CostBudget : public RB_ObjectContainer {

public:
    ACC_CostBudget(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_CostBudget(ACC_CostBudget* obj);
    virtual ~ACC_CostBudget();

private:
    void createMembers();

};

#endif // ACC_COSTBUDGET_H
