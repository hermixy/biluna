/*****************************************************************
 * $Id: peng_typesubsection.h 1826 2012-12-08 00:31:33Z rutger $
 *
 * Copyright (C) 2007 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_TYPESUBSECTION_H
#define PENG_TYPESUBSECTION_H

#include "rb_objectcontainer.h"


/**
 * Class that holds the sub section data of equipment types that can be used
 * on a datasheet.
 * For example a shell and tube heat exchanger has a shell and
 * a tube side which in turn can have a inlet and a outlet side. The inlet and 
 * outlet side can have different design conditions.
 */
class PENG_TypeSubsection : public RB_ObjectContainer {
    
public:
    PENG_TypeSubsection(const RB_String& id = "", RB_ObjectBase* p = 0,
                        const RB_String& n = "", RB_ObjectFactory* f = NULL);
	PENG_TypeSubsection(PENG_TypeSubsection* typeSubsection);
    virtual ~PENG_TypeSubsection ();
    
private:
	void createMembers();

};

#endif /*PENG_TYPESUBSECTION_H*/
