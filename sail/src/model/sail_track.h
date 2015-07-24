/*****************************************************************
 * $Id: sail_track.h 1725 2012-07-29 14:25:31Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_TRACK_H
#define SAIL_TRACK_H

#include "rb_objectatomic.h"

/**
  Class for tracks. A track is a set of coordinates, a 'From' coordinate
  and a 'To' coordinate. The purpose is to valid the speed for the
  individual track.
 */
class SAIL_Track : public RB_ObjectAtomic {

public:
    SAIL_Track(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SAIL_Track(SAIL_Track* obj);
    virtual ~SAIL_Track();

private:
    void createMembers();

};

#endif // SAIL_TRACK_H
