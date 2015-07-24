/*****************************************************************
 * $Id: sail_symbol.h 1723 2012-07-27 14:42:24Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_SYMBOL_H
#define SAIL_SYMBOL_H

#include "rb_objectatomic.h"

/**
 * Class for map symbols and beacons
 */
class SAIL_Symbol : public RB_ObjectAtomic {

public:
    SAIL_Symbol(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SAIL_Symbol(SAIL_Symbol* obj);
    virtual ~SAIL_Symbol();

private:
    void createMembers();

};

#endif // SAIL_SYMBOL_H
