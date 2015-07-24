/*****************************************************************
 * $Id: acc_accountgroup.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Feb 22, 2010 6:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACCOUNTGROUP_H
#define ACC_ACCOUNTGROUP_H

#include "rb_objectcontainer.h"

/**
 * Class for account groups
 */
class ACC_AccountGroup : public RB_ObjectContainer {

public:
    ACC_AccountGroup(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_AccountGroup(ACC_AccountGroup* obj);
    virtual ~ACC_AccountGroup();

private:
    void createMembers();

};

#endif // ACC_ACCOUNTGROUP_H
