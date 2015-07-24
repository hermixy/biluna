/*****************************************************************
 * $Id: crm_campaigntype.h 1545 2012-02-11 17:34:25Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_CAMPAIGNTYPE_H
#define CRM_CAMPAIGNTYPE_H

#include "rb_objectcontainer.h"

/**
 * Class for campaign type, such as mass email, cold calls
 */
class CRM_CampaignType : public RB_ObjectContainer {

public:
    CRM_CampaignType(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    CRM_CampaignType(CRM_CampaignType* obj);
    virtual ~CRM_CampaignType();

private:
    void createMembers();

};

#endif // CRM_CAMPAIGNTYPE_H
