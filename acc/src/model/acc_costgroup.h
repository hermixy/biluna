/*****************************************************************
 * $Id: acc_costgroup.h 1677 2012-06-20 20:32:29Z rutger $
 * Created: Jun 20, 2012 6:04:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_COSTGROUP_H
#define ACC_COSTGROUP_H

#include "rb_objectcontainer.h"

/**
 * Class for cost group of cost centers. Cost group can be
 * an alternative parent for the cost centers, for reporting
 * purposes
 */
class ACC_CostGroup : public RB_ObjectContainer {

public:
    ACC_CostGroup(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_CostGroup(ACC_CostGroup* obj);
    virtual ~ACC_CostGroup();

private:
    void createMembers();

};

#endif // ACC_COSTGROUP_H
