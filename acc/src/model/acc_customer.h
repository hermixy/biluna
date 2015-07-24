/*****************************************************************
 * $Id: acc_customer.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Dec 23, 2009 9:04:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CUSTOMER_H
#define ACC_CUSTOMER_H

#include "rb_objectcontainer.h"

/**
 * Class for customer branch
 */
class ACC_Customer : public RB_ObjectContainer {

public:
    ACC_Customer(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_Customer(ACC_Customer* branch);
    virtual ~ACC_Customer();

private:
    void createMembers();

};

#endif // ACC_CUSTOMER_H
