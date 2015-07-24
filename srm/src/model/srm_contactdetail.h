/*****************************************************************
 * $Id: srm_contactdetail.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Mar 1, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_CONTACTDETAIL_H
#define SRM_CONTACTDETAIL_H

#include "rb_objectatomic.h"

/**
 * Class for the contact (sales and private) details
 */
class SRM_ContactDetail : public RB_ObjectAtomic {

public:
    SRM_ContactDetail(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SRM_ContactDetail(SRM_ContactDetail* obj);
    virtual ~SRM_ContactDetail();

private:
    void createMembers();

};

#endif // SRM_CONTACTDETAIL_H
