/*****************************************************************
 * $Id: crm_leadsourcetype.h 1545 2012-02-11 17:34:25Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_LEADSOURCETYPE_H
#define CRM_LEADSOURCETYPE_H

#include "rb_objectcontainer.h"

/**
 * Class for lead source type, such as website, seminar, self generated
 */
class CRM_LeadSourceType : public RB_ObjectContainer {

public:
    CRM_LeadSourceType(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    CRM_LeadSourceType(CRM_LeadSourceType* obj);
    virtual ~CRM_LeadSourceType();

private:
    void createMembers();

};

#endif // CRM_LEADSOURCETYPE_H
