/*****************************************************************
 * $Id: srm_campaigntarget.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Mar 5, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_CAMPAIGNTARGET_H
#define SRM_CAMPAIGNTARGET_H

#include "rb_objectatomic.h"

/**
 * Class for sales campaign targets or groups of contacts
 */
class SRM_CampaignTarget : public RB_ObjectAtomic {

public:
    SRM_CampaignTarget(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SRM_CampaignTarget(SRM_CampaignTarget* obj);
    virtual ~SRM_CampaignTarget();

private:
    void createMembers();

};

#endif // SRM_CAMPAIGNTARGET_H
