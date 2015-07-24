/*****************************************************************
 * $Id: acc_assetsupplattr.h 1473 2011-10-24 17:16:18Z rutger $
 * Created: Oct 24, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ASSETSUPPLATTR
#define ACC_ASSETSUPPLATTR

#include "rb_objectcontainer.h"

/**
 * Class for asset/supplier attribute, contract item
 * between supplier and asset
 */
class ACC_AssetSupplAttr : public RB_ObjectContainer {

public:
    ACC_AssetSupplAttr(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_AssetSupplAttr(ACC_AssetSupplAttr* branch);
    virtual ~ACC_AssetSupplAttr();

private:
    void createMembers();

};

#endif // ACC_ASSETSUPPLATTR
