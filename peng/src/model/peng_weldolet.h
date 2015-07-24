/*****************************************************************
 * $Id: peng_weldolet.h 1826 2012-12-08 00:31:33Z rutger $
 * Created: Apr 15, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_WELDOLET_H
#define PENG_WELDOLET_H

#include "rb_objectcontainer.h"

/**
 * Class for weldolet weld volume calculations
 */
class PENG_Weldolet : public RB_ObjectContainer {

public:
    PENG_Weldolet(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    PENG_Weldolet(PENG_Weldolet* obj);
    virtual ~PENG_Weldolet();

private:
    void createMembers();

};

#endif // PENG_WELDOLET_H
