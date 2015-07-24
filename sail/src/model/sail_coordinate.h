/*****************************************************************
 * $Id: sail_coordinate.h 1723 2012-07-27 14:42:24Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_COORDINATE_H
#define SAIL_COORDINATE_H

#include "rb_objectatomic.h"

/**
  Class for earth coordinates.
  The two most common datums in use today are WGS 84 (World Geodetic
  System of 1984. A geocentric datum and geographic coordinate system
  created by the United States military) and NAD83 (North American Datum
  of 1983).
 */
class SAIL_Coordinate : public RB_ObjectAtomic {

public:
    SAIL_Coordinate(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SAIL_Coordinate(SAIL_Coordinate* obj);
    virtual ~SAIL_Coordinate();

private:
    void createMembers();

};

#endif // SAIL_COORDINATE_H
