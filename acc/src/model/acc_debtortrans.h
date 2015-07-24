/*****************************************************************
 * $Id: acc_debtortrans.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Feb 27, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_DEBTORTRANS_H
#define ACC_DEBTORTRANS_H

#include "rb_objectcontainer.h"

/**
 * Class for debtor transactions, such invoices to customer
 */
class ACC_DebtorTrans : public RB_ObjectContainer {

public:
    ACC_DebtorTrans(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_DebtorTrans(ACC_DebtorTrans* obj);
    virtual ~ACC_DebtorTrans();

private:
    void createMembers();

};

#endif // ACC_DEBTORTRANS_H
