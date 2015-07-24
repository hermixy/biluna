/*****************************************************************
 * $Id: crm_group.h 1561 2012-03-05 22:18:15Z rutger $
 * Created: Mar 05, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_GROUP_H
#define CRM_GROUP_H

#include "rb_objectcontainer.h"

/**
 * Class for sales (campaign target) group
 */
class CRM_Group : public RB_ObjectContainer {

public:
    CRM_Group(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    CRM_Group(CRM_Group* obj);
    virtual ~CRM_Group();

private:
    void createMembers();

};

#endif // CRM_GROUP_H
