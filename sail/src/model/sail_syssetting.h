/*****************************************************************
 * $Id: sail_syssetting.h 1723 2012-07-27 14:42:24Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_SYSSETTING_H
#define SAIL_SYSSETTING_H

#include "rb_objectcontainer.h"

/**
 * Dummy class used as placeholder for system setting relational table objects
 * The parent ID is therefore not used. Examples are:
 *  SAIL_Map and SAIL_Symbol
 */
class SAIL_SysSetting : public RB_ObjectContainer {

public:
    SAIL_SysSetting(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SAIL_SysSetting(SAIL_SysSetting* obj);
    virtual ~SAIL_SysSetting();

private:
    void createMembers();

};

#endif // SAIL_SYSSETTING_H
