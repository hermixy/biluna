/*****************************************************************
 * $Id: acc_location.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Feb 18, 2010 5:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_LOCATION_H
#define ACC_LOCATION_H

#include "rb_objectcontainer.h"

/**
 * Class for warehouse location
 */
class ACC_Location : public RB_ObjectContainer {

public:
    ACC_Location(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_Location(ACC_Location* obj);
    virtual ~ACC_Location();

private:
    void createMembers();

};

#endif // ACC_LOCATION_H
