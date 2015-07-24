/*****************************************************************
 * $Id: acc_salestaxposting.h 1522 2012-01-19 20:24:58Z rutger $
 * Created: Jan 19, 2012 13:04:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SALESTAXPOSTING_H
#define ACC_SALESTAXPOSTING_H

#include "rb_objectcontainer.h"

/**
 * Class for sales tax postings
 */
class ACC_SalesTaxPosting : public RB_ObjectContainer {

public:
    ACC_SalesTaxPosting(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_SalesTaxPosting(ACC_SalesTaxPosting* obj);
    virtual ~ACC_SalesTaxPosting();

private:
    void createMembers();

};

#endif // ACC_SALESTAXPOSTING_H
