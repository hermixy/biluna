/*****************************************************************
 * $Id: peng_cone.h 1826 2012-12-08 00:31:33Z rutger $
 * Created: Aug 10, 2011 8:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_CONE_H
#define PENG_CONE_H

#include "rb_objectcontainer.h"

/**
 * Class for cone
 */
class PENG_Cone : public RB_ObjectContainer {

public:
    PENG_Cone(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    PENG_Cone(PENG_Cone* obj);
    virtual ~PENG_Cone();

private:
    void createMembers();

};

#endif // PENG_CONE_H
