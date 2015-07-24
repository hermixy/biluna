/*****************************************************************
 * $Id: srm_campaign.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_CAMPAIGN_H
#define SRM_CAMPAIGN_H

#include "rb_objectcontainer.h"

/**
 * Class for sales campaign
 */
class SRM_Campaign : public RB_ObjectContainer {

public:
    SRM_Campaign(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SRM_Campaign(SRM_Campaign* obj);
    virtual ~SRM_Campaign();

private:
    void createMembers();

};

#endif // SRM_CAMPAIGN_H
