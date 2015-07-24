/*****************************************************************
 * $Id: crm_syssetting.h 1545 2012-02-11 17:34:25Z rutger $
 * Created: Feb 12, 2012 16:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_SYSSETTING_H
#define CRM_SYSSETTING_H

#include "rb_objectcontainer.h"

/**
 * Dummy class used as placeholder for system setting relational table objects
 * The parent ID is therefor not used. Examples are:
 *  CRM_CampaignType and CRM_LeadSourceType
 */
class CRM_SysSetting : public RB_ObjectContainer {

public:
    CRM_SysSetting(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    CRM_SysSetting(CRM_SysSetting* obj);
    virtual ~CRM_SysSetting();

private:
    void createMembers();

};

#endif // CRM_SYSSETTING_H
