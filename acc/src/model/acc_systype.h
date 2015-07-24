/*****************************************************************
 * $Id: acc_systype.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Jan 20, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SYSTYPE_H
#define ACC_SYSTYPE_H

#include "rb_objectcontainer.h"

/**
 * Class for system types, which are counters of transactions. For example
 * the last used invoice number is maintained here. During the creation of a
 * new invoice the last invoice number is retrieved from this class and updated
 * with 1 to represent to new invoice number.
 */
class ACC_SysType : public RB_ObjectContainer {

public:
    ACC_SysType(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_SysType(ACC_SysType* obj);
    virtual ~ACC_SysType();

private:
    void createMembers();

};

#endif // ACC_SYSTYPE_H
