/*****************************************************************
 * $Id: srm_project.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_PROJECT_H
#define SRM_PROJECT_H

#include "rb_objectcontainer.h"

/**
 * Class for CRM account or -project
 */
class SRM_Project : public RB_ObjectContainer {

public:
    SRM_Project(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SRM_Project(SRM_Project* obj);
    virtual ~SRM_Project();

private:
    void createMembers();

};

#endif // SRM_PROJECT_H
