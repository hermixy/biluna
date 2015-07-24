/*****************************************************************
 * $Id: crm_campaign.h 1545 2012-02-11 17:34:25Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_CAMPAIGN_H
#define CRM_CAMPAIGN_H

#include "rb_objectcontainer.h"

/**
 * Class for sales campaign
 */
class CRM_Campaign : public RB_ObjectContainer {

public:
    CRM_Campaign(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    CRM_Campaign(CRM_Campaign* obj);
    virtual ~CRM_Campaign();

private:
    void createMembers();

};

#endif // CRM_CAMPAIGN_H
