/*****************************************************************
 * $Id: acc_price.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Jan 20, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_PRICE_H
#define ACC_PRICE_H

#include "rb_objectcontainer.h"

/**
 * Class for item (selling) price
 */
class ACC_Price : public RB_ObjectContainer {

public:
    ACC_Price(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_Price(ACC_Price* obj);
    virtual ~ACC_Price();

private:
    void createMembers();

};

#endif // ACC_PRICE_H
