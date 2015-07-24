/*****************************************************************
 * $Id: acc_unitofmeasure.h 2165 2014-09-24 19:22:36Z rutger $
 * Created: Jan 16, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_UNITOFMEASURE_H
#define ACC_UNITOFMEASURE_H

#include "rb_objectcontainer.h"

/**
 * Class for unit of measurement
 */
class ACC_UnitOfMeasure : public RB_ObjectContainer {

public:
    ACC_UnitOfMeasure(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_UnitOfMeasure(ACC_UnitOfMeasure* obj);
    virtual ~ACC_UnitOfMeasure();

private:
    void createMembers();

};

#endif // ACC_UNITOFMEASURE_H
