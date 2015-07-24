/*****************************************************************
 * $Id: acc_salesman.h 1380 2011-03-07 20:47:51Z rutger $
 * Created: Feb 20, 2010 12:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SALESMAN_H
#define ACC_SALESMAN_H

#include "rb_objectcontainer.h"

/**
 * Class for salesmen
 */
class ACC_SalesMan : public RB_ObjectContainer {

public:
    ACC_SalesMan(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_SalesMan(ACC_SalesMan* obj);
    virtual ~ACC_SalesMan();

private:
    void createMembers();

};

#endif // ACC_SALESMAN_H
