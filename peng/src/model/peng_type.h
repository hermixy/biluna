/*****************************************************************
 * $Id: peng_type.h 1826 2012-12-08 00:31:33Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_TYPE_H
#define PENG_TYPE_H

#include "rb_objectcontainer.h"


/**
 * Class that holds the different types of equipment that can be used in 
 * this project, for example 'S&T Heat Exchanger'.
 */
class PENG_Type : public RB_ObjectContainer {
    
public:
    PENG_Type(const RB_String& id = "", RB_ObjectBase* p = 0,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
	PENG_Type(PENG_Type* type);
    virtual ~PENG_Type ();
    
     
private:
	
	void createMembers();

};

#endif /*PENG_TYPE_H*/
