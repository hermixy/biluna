/*****************************************************************
 * $Id: peng_component.cpp 1826 2012-12-08 00:31:33Z rutger $
 *
 * Copyright (C) 2007 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_component.h"


PENG_Component::PENG_Component(const RB_String& id, RB_ObjectContainer* p,
                             const RB_String& n, RB_ObjectFactory* f)
                                 : /*RB_Component*/RB_ObjectContainer(id, p, n, f) {
     /* for derived classes such as PENG_Cylinder and PENG_Elbow
	 setStandardPortCount(x);
	 createMembers();
	 */
}


PENG_Component::PENG_Component(PENG_Component* component)
            : /*RB_Component*/RB_ObjectContainer(component) {
    /* for derived classes such as PENG_Cylinder and PENG_Elbow
	setStandardPortCount(x); 
	createMembers();
	*this = *component;
 	*/
}


PENG_Component::~PENG_Component() {
	
}


/**
 * Get the atomic type of component, to be implemented by actual components
 * @return atomic type of component
 */
PENG2::CompType PENG_Component::type() {
    // RB_DEBUG->print("PENG_Component::type()");
    return PENG2::CompNone;
}
	

/**
 * Get the complex type of component, to be implemented by actual components. If
 * no complex type this function returns the same as type() or none. A complex 
 * type example is a nozzle, which consists of a flange and a pipe.
 * @return complex type of component
 */
PENG2::CompType PENG_Component::complexType() {
    // RB_DEBUG->print("PENG_Component::complexType()");
	RB_ObjectMember* mem = getMember("complexType");
	
	if (mem) {
        return (PENG2::CompType)mem->getValue().toInt();
	}
	
    return PENG2::CompNone;
}


/**
 * Set complex type of component such as nozzle. The enumerators are passed
 * as an integer for flexibility and multi purpose reasons
 * @param cType complex type of component
 */
void PENG_Component::setComplexType(PENG2::CompType cType) {
    // RB_DEBUG->print("PENG_Component::setComplexType()");
	setValue("complexType", (int)cType);
}

/**
 * @return true if component type accepts custom ports
 */
bool PENG_Component::acceptCustomPort() {
	switch (type()) {
        case PENG2::CompBend:
			return true;
			break;
        case PENG2::CompBlock:
			// TODO: implement
			return false;
			break;
        case PENG2::CompCone:
			return true;
			break;
        case PENG2::CompCylinder:
			return true;
			break;
        case PENG2::CompDisc:
			return true;
			break;
        case PENG2::CompHalfSphere:
			return true;
			break;
        case PENG2::CompHead:
			return true;
			break;
        case PENG2::CompRing:
			return true;
			break;
		default:
			break;
	}
	
	return false;
}
