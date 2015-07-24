/*****************************************************************
 * $Id: acc_assetcustattr.h 1473 2011-10-24 17:16:18Z rutger $
 * Created: Sept 18, 2012 1:42:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CUSTCOSTPART_H
#define ACC_CUSTCOSTPART_H

#include "rb_objectatomic.h"

/**
 * Class for customer part of cost (profit) center value
 */
class ACC_CustCostPart : public RB_ObjectAtomic {

public:
    ACC_CustCostPart(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_CustCostPart(ACC_CustCostPart* branch);
    virtual ~ACC_CustCostPart();

private:
    void createMembers();

};

#endif // ACC_CUSTCOSTPART_H
