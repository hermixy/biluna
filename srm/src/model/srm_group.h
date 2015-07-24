/*****************************************************************
 * $Id: srm_group.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Mar 05, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_GROUP_H
#define SRM_GROUP_H

#include "rb_objectcontainer.h"

/**
 * Class for sales (campaign target) group
 */
class SRM_Group : public RB_ObjectContainer {

public:
    SRM_Group(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SRM_Group(SRM_Group* obj);
    virtual ~SRM_Group();

private:
    void createMembers();

};

#endif // SRM_GROUP_H
