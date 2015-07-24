/*****************************************************************
 * $Id: peng_insulationhci.h 2165 2014-09-24 19:22:36Z rutger $
 * Created: Jul 9, 2005 3:55:29 PM - rutger
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_INSULATIONHCI_H
#define PENG_INSULATIONHCI_H

#include "rb_objectatomic.h"

/**
 * Class for data of heat conservation indoor insulation
 */
class PENG_InsulationHCI : public RB_ObjectAtomic {

public:
    PENG_InsulationHCI(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    PENG_InsulationHCI(PENG_InsulationHCI* obj);
    virtual ~PENG_InsulationHCI();

private:
    void createMembers();

};

#endif // PENG_INSULATIONHCI_H
