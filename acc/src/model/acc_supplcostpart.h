/*****************************************************************
 * $Id: acc_assetcustattr.h 1473 2011-10-24 17:16:18Z rutger $
 * Created: Sept 18, 2012 1:42:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SUPPLCOSTPART_H
#define ACC_SUPPLCOSTPART_H

#include "rb_objectatomic.h"

/**
 * Class for supplier part of cost (profit) center value
 */
class ACC_SupplCostPart : public RB_ObjectAtomic {

public:
    ACC_SupplCostPart(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_SupplCostPart(ACC_SupplCostPart* branch);
    virtual ~ACC_SupplCostPart();

private:
    void createMembers();

};

#endif // ACC_SUPPLCOSTPART_H
