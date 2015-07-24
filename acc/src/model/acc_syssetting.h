/*****************************************************************
 * $Id: acc_syssetting.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Dec 13, 2011 10:04:25 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SYSSETTING_H
#define ACC_SYSSETTING_H

#include "rb_objectcontainer.h"

/**
 * Dummy class used as placeholder for system setting relational table objects
 * The parent ID is therefor not used. Examples are:
 *  ACC_Area (sales area), ACC_AccountControl and ACC_Currency
 */
class ACC_SysSetting : public RB_ObjectContainer {

public:
    ACC_SysSetting(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_SysSetting(ACC_SysSetting* obj);
    virtual ~ACC_SysSetting();

private:
    void createMembers();

};

#endif // ACC_SYSSETTING_H
