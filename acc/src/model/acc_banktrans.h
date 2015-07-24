/*****************************************************************
 * $Id: acc_banktrans.h 1380 2011-03-07 20:47:51Z rutger $
 * Created: Sep 2, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_BANKTRANS_H
#define ACC_BANKTRANS_H

#include "rb_objectcontainer.h"

/**
 * Class for bank transactions such as bank receipts and payments
 */
class ACC_BankTrans : public RB_ObjectContainer {

public:
    ACC_BankTrans(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_BankTrans(ACC_BankTrans* obj);
    virtual ~ACC_BankTrans();

private:
    void createMembers();

};

#endif // ACC_BANKTRANS_H
