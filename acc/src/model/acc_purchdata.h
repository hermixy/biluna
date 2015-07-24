/*****************************************************************
 * $Id: acc_purchdata.h 1380 2011-03-07 20:47:51Z rutger $
 * Created: May 26, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_PURCHDATA_H
#define ACC_PURCHDATA_H

#include "rb_objectcontainer.h"

/**
 * Class for supplier purchase data
 */
class ACC_PurchData : public RB_ObjectContainer {

public:
    ACC_PurchData(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_PurchData(ACC_PurchData* obj);
    virtual ~ACC_PurchData();

private:
    void createMembers();

};

#endif // ACC_PURCHDATA_H
