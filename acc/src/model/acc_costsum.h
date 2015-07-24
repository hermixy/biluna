/*****************************************************************
 * $Id: acc_costsum.h 1681 2012-06-25 21:36:26Z rutger $
 * Created: Jun 24, 2012 1:50:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_COSTSUM_H
#define ACC_COSTSUM_H

#include "rb_objectatomic.h"

/**
 * Class for cost/profit center (monthly) summary
 */
class ACC_CostSum : public RB_ObjectAtomic {

public:
    ACC_CostSum(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_CostSum(ACC_CostSum* obj);
    virtual ~ACC_CostSum();

private:
    void createMembers();

};

#endif // ACC_COSTSUM_H
