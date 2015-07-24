/*****************************************************************
 * $Id: peng_fluidstability.h 2154 2014-07-30 19:44:11Z rutger $
 * Created: 2014-07-29 - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_FLUIDSTABILITY_H
#define PENG_FLUIDSTABILITY_H

#include "rb_objectatomic.h"

/**
 * Class for hazard group of line or equipment
 */
class PENG_FluidStability : public RB_ObjectAtomic {
    
public:
    PENG_FluidStability(const RB_String& id = "", RB_ObjectBase* p = NULL,
                     const RB_String& n = "", RB_ObjectFactory* f = NULL);
    PENG_FluidStability(PENG_FluidStability* obj);
    virtual ~PENG_FluidStability ();
     
private:
    void createMembers();

};
#endif //PENG_FLUIDSTABILITY_H
