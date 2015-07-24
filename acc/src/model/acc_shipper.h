/*****************************************************************
 * $Id: acc_shipper.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Feb 20, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SHIPPER_H
#define ACC_SHIPPER_H

#include "rb_objectcontainer.h"

/**
 * Class for shipper or transport company
 */
class ACC_Shipper : public RB_ObjectContainer {

public:
    ACC_Shipper(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_Shipper(ACC_Shipper* obj);
    virtual ~ACC_Shipper();

private:
    void createMembers();

};

#endif // ACC_SHIPPER_H
