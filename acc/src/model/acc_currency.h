/*****************************************************************
 * $Id: acc_currency.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Jan 20, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CURRENCY_H
#define ACC_CURRENCY_H

#include "rb_objectcontainer.h"

/**
 * Class for currency
 */
class ACC_Currency : public RB_ObjectContainer {

public:
    ACC_Currency(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_Currency(ACC_Currency* obj);
    virtual ~ACC_Currency();

private:
    void createMembers();

};

#endif // ACC_CURRENCY_H
