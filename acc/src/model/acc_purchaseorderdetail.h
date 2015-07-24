/*****************************************************************
 * $Id: acc_purchaseorderdetail.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Sept 22, 2011 5:04:25 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_PURCHASEORDERDETAIL_H
#define ACC_PURCHASEORDERDETAIL_H

#include "rb_objectcontainer.h"

/**
 * Class for purchase order details
 */
class ACC_PurchaseOrderDetail : public RB_ObjectContainer {

public:
    ACC_PurchaseOrderDetail(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_PurchaseOrderDetail(ACC_PurchaseOrderDetail* obj);
    virtual ~ACC_PurchaseOrderDetail();

private:
    void createMembers();

};

#endif // ACC_PURCHASEORDERDETAIL_H
