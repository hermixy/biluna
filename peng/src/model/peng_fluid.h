/*****************************************************************
 * $Id: peng_fluid.h 2146 2014-05-23 20:27:06Z rutger $
 * Created 2006 Rutger
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_FLUID_H
#define PENG_FLUID_H

#include "rb_objectatomic.h"

/**
 * Holds the data such as abbreviation, name, chemical properties and 
 * hazard classification of the process fluids in this project. The hazard 
 * classification is for example necessary for the PED category determination.
 */
class PENG_Fluid : public RB_ObjectAtomic {

public:
    PENG_Fluid(const RB_String& id = "", RB_ObjectBase* p = NULL,
               const RB_String& n = "", RB_ObjectFactory* f = NULL);
    PENG_Fluid(PENG_Fluid* fluid);
    virtual ~PENG_Fluid();
    
private:
	void createMembers();

};

#endif /*PENG_FLUID_H*/
