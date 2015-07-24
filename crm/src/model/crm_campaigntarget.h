/*****************************************************************
 * $Id: crm_campaigntarget.h 1561 2012-03-05 22:18:15Z rutger $
 * Created: Mar 5, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_CAMPAIGNTARGET_H
#define CRM_CAMPAIGNTARGET_H

#include "rb_objectatomic.h"

/**
 * Class for sales campaign targets or groups of contacts
 */
class CRM_CampaignTarget : public RB_ObjectAtomic {

public:
    CRM_CampaignTarget(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    CRM_CampaignTarget(CRM_CampaignTarget* obj);
    virtual ~CRM_CampaignTarget();

private:
    void createMembers();

};

#endif // CRM_CAMPAIGNTARGET_H
