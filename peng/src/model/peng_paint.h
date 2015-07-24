/*****************************************************************
 * $Id: peng_paint.h 2162 2014-08-31 08:49:37Z rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the Plant Engineering (PENG) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_PAINT_H
#define PENG_PAINT_H

#include "rb_objectatomic.h"


/**
 * Class PENG_Paint
 * Paint system which is part of the project paint specification
 */
class PENG_Paint : public RB_ObjectAtomic {

public:
    PENG_Paint(const RB_String& id = "", RB_ObjectBase* p = 0,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    PENG_Paint(PENG_Paint* paint);
    virtual ~PENG_Paint();
    
    
private:
	void createMembers();
    
};
#endif //PENG_PAINT_H

