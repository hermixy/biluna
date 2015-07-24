/*****************************************************************
 * $Id: peng_pipeclasspt.h 2179 2014-09-25 14:22:50Z rutger $
 * Created: Oct 16, 2005 1:20:39 PM - rutger
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_PIPECLASSPT_H_
#define _PENG_PIPECLASSPT_H_

#include "rb_objectatomic.h"


/**
 * Atomic object (no children) containing the pressure/temperature 
 * combinations defined in the (parent) pipe class. Used for checking the 
 * pressure/temperature combinations (PTC) defined in the pipe class with 
 * the sizes in the line table of LDT (this application)
 */
class PENG_PipeClassPT : public RB_ObjectAtomic {
	
public:
    PENG_PipeClassPT(const RB_String& id = "", RB_ObjectBase* p = 0,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    PENG_PipeClassPT(PENG_PipeClassPT* pipeClassPt);
    virtual ~PENG_PipeClassPT();
    

private:

	void createMembers();

};

#endif //PENG_PIPECLASSPT_H
