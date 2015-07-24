/*****************************************************************
 * $Id: srm_syssetting.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Feb 12, 2012 16:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_SYSSETTING_H
#define SRM_SYSSETTING_H

#include "rb_objectcontainer.h"

/**
 * Dummy class used as placeholder for system setting relational table objects
 * The parent ID is therefor not used. Examples are:
 *  SRM_CampaignType and SRM_LeadSourceType
 */
class SRM_SysSetting : public RB_ObjectContainer {

public:
    SRM_SysSetting(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SRM_SysSetting(SRM_SysSetting* obj);
    virtual ~SRM_SysSetting();

private:
    void createMembers();

};

#endif // SRM_SYSSETTING_H
