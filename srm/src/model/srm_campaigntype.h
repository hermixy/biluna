/*****************************************************************
 * $Id: srm_campaigntype.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_CAMPAIGNTYPE_H
#define SRM_CAMPAIGNTYPE_H

#include "rb_objectcontainer.h"

/**
 * Class for campaign type, such as mass email, cold calls
 */
class SRM_CampaignType : public RB_ObjectContainer {

public:
    SRM_CampaignType(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SRM_CampaignType(SRM_CampaignType* obj);
    virtual ~SRM_CampaignType();

private:
    void createMembers();

};

#endif // SRM_CAMPAIGNTYPE_H
