/*****************************************************************
 * $Id: peng_typemember.h 1826 2012-12-08 00:31:33Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_TYPEMEMBER_H
#define PENG_TYPEMEMBER_H

#include "rb_objectcontainer.h"


/**
 * Class that holds the different members of equipment that can be used in 
 * this project. This class is also used to fill the Datasheet widget in the
 * DatasheetDialog of this EQL Equipment List application
 */
class PENG_TypeMember : public RB_ObjectContainer {
    
public:
    PENG_TypeMember(const RB_String& id = "", RB_ObjectBase* p = 0,
                    const RB_String& n = "", RB_ObjectFactory* f = NULL);
	PENG_TypeMember(PENG_TypeMember* member);
    virtual ~PENG_TypeMember ();
    
     
private:
	
	void createMembers();

};

#endif /*PENG_TYPEMEMBER_H*/
