/*****************************************************************
 * $Id: crm_groupcontact.h 1561 2012-03-05 22:18:15Z rutger $
 * Created: Mar 05, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_GROUPCONTACT_H
#define CRM_GROUPCONTACT_H

#include "rb_objectatomic.h"

/**
 * Class for group contact in a sales campaign
 */
class CRM_GroupContact : public RB_ObjectAtomic {

public:
    CRM_GroupContact(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    CRM_GroupContact(CRM_GroupContact* obj);
    virtual ~CRM_GroupContact();

private:
    void createMembers();

};

#endif // CRM_GROUPCONTACT_H
