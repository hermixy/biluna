/*****************************************************************
 * $Id: peng_pipeclass.h 2147 2014-06-04 13:19:44Z rutger $
 * Created: 2005 Rutger
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_PIPECLASS_H
#define PENG_PIPECLASS_H

#include "rb_objectcontainer.h"


/**
 * Container for relevant data of pipe class in PCC (Pipe Class Component) 
 * application for the line table in this PENG (Line Designation Table)
 * application.
 */
class PENG_PipeClass : public RB_ObjectContainer {
	
public:
    PENG_PipeClass(const RB_String& id = "", RB_ObjectBase* p = 0,
                   const RB_String& n = "", RB_ObjectFactory* f = NULL);
    PENG_PipeClass(PENG_PipeClass* pipeClass);
    virtual ~PENG_PipeClass();


private:

	virtual void createMembers();
};

#endif //PENG_PIPECLASS_H
