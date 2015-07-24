/*****************************************************************
 * $Id: sail_route.h 1725 2012-07-29 14:25:31Z rutger $
 * Created: Jul 28, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_ROUTE_H
#define SAIL_ROUTE_H

#include "rb_objectcontainer.h"

/**
  Class for routes. Routes are a series of (route) coordinates.
 */
class SAIL_Route : public RB_ObjectContainer {

public:
    SAIL_Route(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SAIL_Route(SAIL_Route* obj);
    virtual ~SAIL_Route();

private:
    void createMembers();

};

#endif // SAIL_ROUTE_H
