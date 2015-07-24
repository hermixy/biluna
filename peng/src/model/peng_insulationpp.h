/*****************************************************************
 * $Id: peng_insulationpp.h 2165 2014-09-24 19:22:36Z rutger $
 * Created: Jul 9, 2005 3:55:29 PM - rutger
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_INSULATIONPP_H
#define PENG_INSULATIONPP_H

#include "rb_objectatomic.h"

/**
 * Class for data of personnel protection
 */
class PENG_InsulationPP : public RB_ObjectAtomic {

public:
    PENG_InsulationPP(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    PENG_InsulationPP(PENG_InsulationPP* obj);
    virtual ~PENG_InsulationPP();

private:
    void createMembers();

};

#endif // PENG_INSULATIONPP_H
