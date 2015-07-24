/*****************************************************************
 * $Id: acc_transallocn.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Sep 2, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_TRANSALLOCN_H
#define ACC_TRANSALLOCN_H

#include "rb_objectcontainer.h"

/**
 * Class for transaction allocation, payment receipt of invoices to customer
 * or payment done to supplier
 */
class ACC_TransAllocn : public RB_ObjectContainer {

public:
    ACC_TransAllocn(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_TransAllocn(ACC_TransAllocn* obj);
    virtual ~ACC_TransAllocn();

private:
    void createMembers();

};

#endif // ACC_TRANSALLOCN_H
