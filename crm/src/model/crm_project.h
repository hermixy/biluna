/*****************************************************************
 * $Id: crm_project.h 1548 2012-02-16 17:53:00Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_PROJECT_H
#define CRM_PROJECT_H

#include "rb_objectcontainer.h"

/**
 * Class for CRM account or -project
 */
class CRM_Project : public RB_ObjectContainer {

public:
    CRM_Project(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    CRM_Project(CRM_Project* obj);
    virtual ~CRM_Project();

private:
    void createMembers();

};

#endif // CRM_PROJECT_H
