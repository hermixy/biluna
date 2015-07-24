/*****************************************************************
 * $Id: crm_contactdetail.h 1557 2012-03-01 20:48:02Z rutger $
 * Created: Mar 1, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_CONTACTDETAIL_H
#define CRM_CONTACTDETAIL_H

#include "rb_objectatomic.h"

/**
 * Class for the contact (sales and private) details
 */
class CRM_ContactDetail : public RB_ObjectAtomic {

public:
    CRM_ContactDetail(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    CRM_ContactDetail(CRM_ContactDetail* obj);
    virtual ~CRM_ContactDetail();

private:
    void createMembers();

};

#endif // CRM_CONTACTDETAIL_H
