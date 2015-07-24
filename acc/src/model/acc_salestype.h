/*****************************************************************
 * $Id: acc_salestype.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Jan 16, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SALESTYPE_H
#define ACC_SALESTYPE_H

#include "rb_objectcontainer.h"

/**
 * Class for units of measurement
 */
class ACC_SalesType : public RB_ObjectContainer {

public:
    ACC_SalesType(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_SalesType(ACC_SalesType* obj);
    virtual ~ACC_SalesType();

private:
    void createMembers();

};

#endif // ACC_SALESTYPE_H
