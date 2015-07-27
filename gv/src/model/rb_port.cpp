/*****************************************************************
 * $Id: rb_port.cpp 1983 2013-09-02 18:10:02Z rutger $
 *
 * Copyright (C) 2007 Red-Bag. All rights reserved.
 * This file is part of the Biluna gl project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_port.h"


RB_Port::RB_Port(const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic(id, p, n, f) {
	createMembers();
}

RB_Port::RB_Port(RB_Port* port) : RB_ObjectAtomic(port) {
	createMembers();
	*this = *port;
}

RB_Port::~RB_Port() {
	// nothing
}

/**
 * Get the coordinates of the location vector
 */
RVector RB_Port::location() const {
    return RVector(getValue("locX").toDouble(),
                        getValue("locY").toDouble(),
                        getValue("locZ").toDouble());
}

/**
 * Get the coordinates of the direction vector
 */
RVector RB_Port::direction() const {
    return RVector(getValue("dirX").toDouble(),
                        getValue("dirY").toDouble(),
                        getValue("dirZ").toDouble());
}

/**
 * Get the angle of the orientation.
 * Note: in Biluna the node will usually set the orientation
 * @return orientation angle
 */
double RB_Port::orientation() const {
    return getValue("oriAngle").toDouble();
}


/**
 * Set the coordinates of the location vector
 * @param v vector with coordinates
 */	
void RB_Port::setLocation(const RVector& v) {
    setValue("locX", v.x);
    setValue("locY", v.y);
    setValue("locZ", v.z);
}

/**
 * Set the coordinates of the direction vector
 * 
 * @param v vector with coordinates
 */
void RB_Port::setDirection(const RVector& v) {
    setValue("dirX", v.x);
    setValue("dirY", v.y);
    setValue("dirZ", v.z);
}

/**
 * Set the coordinates of the orientation vector
 * Not used, in Biluna the node will usually set the orientation
 * 
 * @param v vector with coordinates
 */
void RB_Port::setOrientation(double angle) {
    setValue("oriAngle", angle);
}

/**
 * Create the members of this class
 */
void RB_Port::createMembers() {
	// Location vector coordinates
    addMember(this, "locX", "mm", 0.0, RB2::MemberDouble);			// 0
    addMember(this, "locY", "mm", 0.0, RB2::MemberDouble);			// 1
    addMember(this, "locZ", "mm", 0.0, RB2::MemberDouble);			// 2
    
    // Direction vector coordinates
    addMember(this, "dirX", "mm", 0.0, RB2::MemberDouble);			// 3
    addMember(this, "dirY", "mm", 0.0, RB2::MemberDouble);			// 4
    addMember(this, "dirZ", "mm", 0.0, RB2::MemberDouble);			// 5
    
    // Orientation angle measured as rotation around direction vector
    addMember(this, "oriAngle", "degree", 0.0, RB2::MemberDouble);	// 6
}
