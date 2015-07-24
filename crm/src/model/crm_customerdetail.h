/*****************************************************************
 * $Id: crm_customerdetail.h 1552 2012-02-19 17:10:45Z rutger $
 * Created: Feb 19, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_CUSTOMERDETAIL_H
#define CRM_CUSTOMERDETAIL_H

#include "rb_objectatomic.h"

/**
 * Class for the customer (sales and private) details
 */
class CRM_CustomerDetail : public RB_ObjectAtomic {

public:
    CRM_CustomerDetail(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    CRM_CustomerDetail(CRM_CustomerDetail* obj);
    virtual ~CRM_CustomerDetail();

private:
    void createMembers();

};

#endif // CRM_CUSTOMERDETAIL_H
