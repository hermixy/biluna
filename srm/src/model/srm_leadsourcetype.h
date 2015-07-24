/*****************************************************************
 * $Id: srm_leadsourcetype.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_LEADSOURCETYPE_H
#define SRM_LEADSOURCETYPE_H

#include "rb_objectcontainer.h"

/**
 * Class for lead source type, such as website, seminar, self generated
 */
class SRM_LeadSourceType : public RB_ObjectContainer {

public:
    SRM_LeadSourceType(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SRM_LeadSourceType(SRM_LeadSourceType* obj);
    virtual ~SRM_LeadSourceType();

private:
    void createMembers();

};

#endif // SRM_LEADSOURCETYPE_H
