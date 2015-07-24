/*****************************************************************
 * $Id: acc_creditortrans.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Sep 2, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CREDITORTRANS_H
#define ACC_CREDITORTRANS_H

#include "rb_objectcontainer.h"

/**
 * Class for creditor transactions, such invoices from supplier
 */
class ACC_CreditorTrans : public RB_ObjectContainer {

public:
    ACC_CreditorTrans(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_CreditorTrans(ACC_CreditorTrans* obj);
    virtual ~ACC_CreditorTrans();

private:
    void createMembers();

};

#endif // ACC_CREDITORTRANS_H
