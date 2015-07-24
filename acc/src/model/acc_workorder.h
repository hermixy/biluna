/*****************************************************************
 * $Id: acc_workorder.h 2191 2014-10-27 20:31:51Z rutger $
 * Created: Oct 27, 2014 11:41:25 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_WORKORDER_H
#define ACC_WORKORDER_H

#include "rb_objectcontainer.h"

/**
 * Class for (internal) work orders
 */
class ACC_WorkOrder : public RB_ObjectContainer {

public:
    ACC_WorkOrder(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_WorkOrder(ACC_WorkOrder* obj);
    virtual ~ACC_WorkOrder();

private:
    void createMembers();

};

#endif // ACC_WORKORDER_H
