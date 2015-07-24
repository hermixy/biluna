/*****************************************************************
 * $Id: pcalc_component.h 1826 2012-12-08 00:31:33Z rutger $
 * Created: Apr 7, 2015 - rutger
 *
 * Copyright (C) 2015 Biluna. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_EN1591_ASSEMBLY_H
#define PCALC_EN1591_ASSEMBLY_H

#include "rb_objectcontainer.h"

/**
 * EN1591 assembly data object, root object for flange, gasket, washer,
 * bolt, bolthole, loadcase and force
 */
class PCALC_EN1591_Assembly : public RB_ObjectContainer  {
	
public:
    PCALC_EN1591_Assembly(const RB_String& id = "", RB_ObjectBase* p = NULL,
                  const RB_String& n = "", RB_ObjectFactory* f = NULL);
    PCALC_EN1591_Assembly(PCALC_EN1591_Assembly* component);
    virtual ~PCALC_EN1591_Assembly();
	
private:
    void createMembers();
};

#endif /* PCALC_EN1591_ASSEMBLY_H */
