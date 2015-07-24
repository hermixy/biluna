/*****************************************************************
 * $Id: peng_pipeclasssize.h 2179 2014-09-25 14:22:50Z rutger $
 * Created: Oct 16, 2005 1:20:39 PM - rutger
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_PIPECLASSSIZE_H
#define PENG_PIPECLASSSIZE_H

#include "rb_objectatomic.h"


/**
 * Atomic object (no children) containing the sizes defined in the (parent) 
 * pipe class. Used for checking the sizes in the pipe class with the sizes
 * in the line table of PENG (this application)
 */
class PENG_PipeClassSize : public RB_ObjectAtomic {
	
public:
    PENG_PipeClassSize(const RB_String& id = "", RB_ObjectBase* p = 0,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    PENG_PipeClassSize(PENG_PipeClassSize* pipeClassSize);
    virtual ~PENG_PipeClassSize();
    

private:

	void createMembers();
	
};

#endif //PENG_PIPECLASSSIZE_H
