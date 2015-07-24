/*****************************************************************
 * $Id: peng_line.h 1826 2012-12-08 00:31:33Z rutger $
 * Created: 2014-06-16 - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_HAZARDGROUP_H
#define PENG_HAZARDGROUP_H

#include "rb_objectatomic.h"

/**
 * Class for hazard group of line or equipment
 */
class PENG_HazardGroup : public RB_ObjectAtomic {
    
public:
    PENG_HazardGroup(const RB_String& id = "", RB_ObjectBase* p = NULL,
                     const RB_String& n = "", RB_ObjectFactory* f = NULL);
    PENG_HazardGroup(PENG_HazardGroup* obj);
    virtual ~PENG_HazardGroup ();
     
private:
    void createMembers();

};
#endif //PENG_HAZARDGROUP_H
