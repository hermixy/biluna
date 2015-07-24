/*****************************************************************
 * $Id: acc_salesglposting.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Apr 02, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SALESGLPOSTING_H
#define ACC_SALESGLPOSTING_H

#include "rb_objectcontainer.h"

/**
 * Class for Sales General Ledger postings
 */
class ACC_SalesGlPosting : public RB_ObjectContainer {

public:
    ACC_SalesGlPosting(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_SalesGlPosting(ACC_SalesGlPosting* obj);
    virtual ~ACC_SalesGlPosting();

private:
    void createMembers();

};

#endif // ACC_SALESGLPOSTING_H
