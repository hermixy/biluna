/*****************************************************************
 * $Id: acc_salesorder.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Jan 23, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SALESORDER_H
#define ACC_SALESORDER_H

#include "rb_objectcontainer.h"

/**
 * Class for sales orders
 */
class ACC_SalesOrder : public RB_ObjectContainer {

public:
    ACC_SalesOrder(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_SalesOrder(ACC_SalesOrder* obj);
    virtual ~ACC_SalesOrder();

private:
    void createMembers();

};

#endif // ACC_SALESORDER_H
