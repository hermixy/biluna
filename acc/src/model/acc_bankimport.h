/*****************************************************************
 * $Id: acc_bankimport.h 1803 2012-11-22 21:43:16Z rutger $
 * Created: Oct 15, 2012 19:41:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_BANKIMPORT_H
#define ACC_BANKIMPORT_H

#include "rb_objectatomic.h"

/**
 * In-memory data class for import of bank statements
 *
 * NOTE: this class is NOT part of the ACC_ObjectFactory
 */
class ACC_BankImport : public RB_ObjectAtomic {

public:
    ACC_BankImport(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_BankImport(ACC_BankImport* obj);
    virtual ~ACC_BankImport();

private:
    void createMembers();

};

#endif // ACC_BANKIMPORT_H
