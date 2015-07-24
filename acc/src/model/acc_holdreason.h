/*****************************************************************
 * $Id: acc_holdreason.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Feb 17, 2010 8:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_HOLDREASON_H
#define ACC_HOLDREASON_H

#include "rb_objectcontainer.h"

/**
 * Class for credit status or hold reasons of customer
 */
class ACC_HoldReason : public RB_ObjectContainer {

public:
    ACC_HoldReason(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_HoldReason(ACC_HoldReason* obj);
    virtual ~ACC_HoldReason();

private:
    void createMembers();

};

#endif // ACC_HOLDREASON_H
