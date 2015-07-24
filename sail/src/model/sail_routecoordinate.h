/*****************************************************************
 * $Id: sail_routecoordinate.h 1725 2012-07-29 14:25:31Z rutger $
 * Created: Jul 28, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_ROUTECOORDINATE_H
#define SAIL_ROUTECOORDINATE_H

#include "rb_objectatomic.h"

/**
  Class for route coordinates.
 */
class SAIL_RouteCoordinate : public RB_ObjectAtomic {

public:
    SAIL_RouteCoordinate(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SAIL_RouteCoordinate(SAIL_RouteCoordinate* obj);
    virtual ~SAIL_RouteCoordinate();

private:
    void createMembers();

};

#endif // SAIL_ROUTECOORDINATE_H
