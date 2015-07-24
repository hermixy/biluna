/*****************************************************************
 * $Id: acc_debtortype.h 1380 2011-03-07 20:47:51Z rutger $
 * Created: Feb 17, 2010 10:14:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_DEBTORTYPE_H
#define ACC_DEBTORTYPE_H

#include "rb_objectcontainer.h"

/**
 * Class for debtor (customer) type
 */
class ACC_DebtorType : public RB_ObjectContainer {

public:
    ACC_DebtorType(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_DebtorType(ACC_DebtorType* obj);
    virtual ~ACC_DebtorType();

private:
    void createMembers();

};

#endif // ACC_DEBTORTYPE_H
