/*****************************************************************
 * $Id: acc_assettype.h 1473 2011-10-24 17:16:18Z rutger $
 * Created: Oct 24, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ASSETTYPE_H
#define ACC_ASSETTYPE_H

#include "rb_objectcontainer.h"

/**
 * Class for asset type
 */
class ACC_AssetType : public RB_ObjectContainer {

public:
    ACC_AssetType(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_AssetType(ACC_AssetType* obj);
    virtual ~ACC_AssetType();

private:
    void createMembers();

};

#endif // ACC_ASSETTYPE_H
