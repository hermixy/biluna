/*****************************************************************
 * $Id: acc_purchaseorder.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Sept 22, 2011 5:44:25 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_PURCHASEORDER_H
#define ACC_PURCHASEORDER_H

#include "rb_objectcontainer.h"

/**
 * Class for purchase orders
 */
class ACC_PurchaseOrder : public RB_ObjectContainer {

public:
    ACC_PurchaseOrder(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_PurchaseOrder(ACC_PurchaseOrder* obj);
    virtual ~ACC_PurchaseOrder();

private:
    void createMembers();

};

#endif // ACC_PURCHASEORDER_H
