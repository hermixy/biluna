/*****************************************************************
 * $Id: pcalc_component.h 1826 2012-12-08 00:31:33Z rutger $
 * Created: Apr 7, 2015 - rutger
 *
 * Copyright (C) 2015 Biluna. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_EN1591_FLANGE_H
#define PCALC_EN1591_FLANGE_H

#include "rb_objectatomic.h"

/**
 * EN1591 flange data object
 */
class PCALC_EN1591_Flange : public RB_ObjectAtomic  {
	
public:
    PCALC_EN1591_Flange(const RB_String& id = "", RB_ObjectBase* p = NULL,
                  const RB_String& n = "", RB_ObjectFactory* f = NULL);
    PCALC_EN1591_Flange(PCALC_EN1591_Flange* component);
    virtual ~PCALC_EN1591_Flange();
	
private:
    void createMembers();
};

#endif /* PCALC_EN1591_FLANGE_H */
