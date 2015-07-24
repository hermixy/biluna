/*****************************************************************
 * $Id: peng_component.h 1826 2012-12-08 00:31:33Z rutger $
 *
 * Copyright (C) 2007 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_COMPONENT_H
#define PENG_COMPONENT_H

#include "peng.h"
// #include "rb_component.h" temporary replaced by
#include "rb_objectcontainer.h"

/**
 * Base EQL class providing base functions of components such as cylinder, 
 * elbow, flange and head for the Equipment List
 */
class PENG_Component : public /*RB_Component*/ RB_ObjectContainer  {
	
public:
    PENG_Component(const RB_String& id = "", RB_ObjectContainer* p = NULL, 
                  const RB_String& n = "", RB_ObjectFactory* f = NULL);
	PENG_Component(PENG_Component* component);
	virtual ~PENG_Component();
	
	virtual PENG2::CompType type();
	virtual PENG2::CompType complexType();
	virtual void setComplexType(PENG2::CompType cType);
	virtual bool acceptCustomPort();
};

#endif /* PENG_COMPONENT_H */
