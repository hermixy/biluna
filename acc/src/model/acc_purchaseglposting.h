/*****************************************************************
 * $Id: acc_purchaseglposting.h 1888 2013-01-31 17:21:31Z rutger $
 * Created: Jan 28, 2013 17:17:25 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_PURCHASEGLPOSTING_H
#define ACC_PURCHASEGLPOSTING_H

#include "rb_objectcontainer.h"

/**
 * Class for Purchase General Ledger postings
 */
class ACC_PurchaseGlPosting : public RB_ObjectContainer {

public:
    ACC_PurchaseGlPosting(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_PurchaseGlPosting(ACC_PurchaseGlPosting* obj);
    virtual ~ACC_PurchaseGlPosting();

private:
    void createMembers();

};

#endif // ACC_PURCHASEGLPOSTING_H
