/*****************************************************************
 * $Id: peng_typesection.h 1826 2012-12-08 00:31:33Z rutger $
 *
 * Copyright (C) 2007 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_TYPESECTION_H
#define PENG_TYPESECTION_H

#include "rb_objectcontainer.h"


/**
 * Class that holds the section of an equipment type that can be used on
 * datasheet.
 * For example a shell and tube heat exchanger has a shell and
 * a tube side which can have different design conditions. A stacked column
 * can have different design conditions for the top and the bottom section
 */
class PENG_TypeSection : public RB_ObjectContainer {
    
public:
    PENG_TypeSection(const RB_String& id = "", RB_ObjectBase* p = 0,
                    const RB_String& n = "", RB_ObjectFactory* f = NULL);
	PENG_TypeSection(PENG_TypeSection* typeSection);
    virtual ~PENG_TypeSection ();
    
private:
	void createMembers();

};

#endif /*PENG_TYPESECTION_H*/
