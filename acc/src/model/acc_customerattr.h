/*****************************************************************
 * $Id: acc_customerattr.h 1476 2011-11-07 22:08:48Z rutger $
 * Created: Oct 24, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CUSTOMERATTR_H
#define ACC_CUSTOMERATTR_H

#include "rb_objectcontainer.h"

/**
 * Class for customer attribute, such as contractual items
 */
class ACC_CustomerAttr : public RB_ObjectContainer {

public:
    ACC_CustomerAttr(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_CustomerAttr(ACC_CustomerAttr* obj);
    virtual ~ACC_CustomerAttr();

private:
    void createMembers();

};

#endif // ACC_CUSTOMERATTR_H
