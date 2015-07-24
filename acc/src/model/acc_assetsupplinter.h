/*****************************************************************
 * $Id: acc_assetsupplinter.h 1476 2011-11-07 22:08:48Z rutger $
 * Created: Oct 24, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ASSETSUPPLINTER_H
#define ACC_ASSETSUPPLINTER_H

#include "rb_objectcontainer.h"

/**
 * Class for asset supplier interface
 */
class ACC_AssetSupplInter : public RB_ObjectContainer {

public:
    ACC_AssetSupplInter(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_AssetSupplInter(ACC_AssetSupplInter* obj);
    virtual ~ACC_AssetSupplInter();

private:
    void createMembers();

};

#endif // ACC_ASSETSUPPLINTER_H
