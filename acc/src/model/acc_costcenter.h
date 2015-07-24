/*****************************************************************
 * $Id: acc_costcenter.h 1677 2012-06-20 20:32:29Z rutger $
 * Created: Jun 20, 2012 6:04:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_COSTCENTER_H
#define ACC_COSTCENTER_H

#include "rb_objectcontainer.h"

/**
 * Class for cost/profit center
 */
class ACC_CostCenter : public RB_ObjectContainer {

public:
    ACC_CostCenter(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_CostCenter(ACC_CostCenter* obj);
    virtual ~ACC_CostCenter();

private:
    void createMembers();

};

#endif // ACC_COSTCENTER_H
