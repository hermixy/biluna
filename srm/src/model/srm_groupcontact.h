/*****************************************************************
 * $Id: srm_groupcontact.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Mar 05, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_GROUPCONTACT_H
#define SRM_GROUPCONTACT_H

#include "rb_objectatomic.h"

/**
 * Class for group contact in a sales campaign
 */
class SRM_GroupContact : public RB_ObjectAtomic {

public:
    SRM_GroupContact(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SRM_GroupContact(SRM_GroupContact* obj);
    virtual ~SRM_GroupContact();

private:
    void createMembers();

};

#endif // SRM_GROUPCONTACT_H
