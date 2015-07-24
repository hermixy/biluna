/*****************************************************************
 * $Id: acc_salesorderdetail.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Jan 23, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SALESORDERDETAIL_H
#define ACC_SALESORDERDETAIL_H

#include "rb_objectcontainer.h"

/**
 * Class for sales order details
 */
class ACC_SalesOrderDetail : public RB_ObjectContainer {

public:
    ACC_SalesOrderDetail(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_SalesOrderDetail(ACC_SalesOrderDetail* obj);
    virtual ~ACC_SalesOrderDetail();

private:
    void createMembers();

};

#endif // ACC_SALESORDERDETAIL_H
