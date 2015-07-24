/*****************************************************************
 * $Id: acc_asset.h 1473 2011-10-24 17:16:18Z rutger $
 * Created: Oct 24, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ASSET_H
#define ACC_ASSET_H

#include "rb_objectcontainer.h"

/**
 * Class for asset, economic resources such building, car, equipment
 */
class ACC_Asset : public RB_ObjectContainer {

public:
    ACC_Asset(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_Asset(ACC_Asset* obj);
    virtual ~ACC_Asset();

private:
    void createMembers();

};

#endif // ACC_ASSET_H
