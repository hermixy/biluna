/*****************************************************************
 * $Id: acc_paymentterm.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Jan 20, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_PAYMENTTERM_H
#define ACC_PAYMENTTERM_H

#include "rb_objectcontainer.h"

/**
 * Class for payment terms
 */
class ACC_PaymentTerm : public RB_ObjectContainer {

public:
    ACC_PaymentTerm(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_PaymentTerm(ACC_PaymentTerm* obj);
    virtual ~ACC_PaymentTerm();

private:
    void createMembers();

};

#endif // ACC_PAYMENTTERM_H
