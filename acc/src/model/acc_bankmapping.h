/*****************************************************************
 * $Id: acc_bankmapping.h 1946 2013-07-01 19:33:25Z rutger $
 * Created: Jul 01, 2013 10:04:25 AM - rutger
 *
 * Copyright (C) 2013 Biluna. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_BANKMAPPING_H
#define ACC_BANKMAPPING_H

#include "rb_objectcontainer.h"

/**
 * Class bank CSV (Comma Separated Value) mapping
 */
class ACC_BankMapping : public RB_ObjectContainer {

public:
    ACC_BankMapping(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_BankMapping(ACC_BankMapping* obj);
    virtual ~ACC_BankMapping();

private:
    void createMembers();

};

#endif // ACC_BANKMAPPING_H
