/*****************************************************************
 * $Id: rb_port.h 1452 2011-09-10 08:19:50Z rutger $
 *
 * Copyright (C) 2007 Red-Bag. All rights reserved.
 * This file is part of the Biluna gl project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_PORT_H
#define RB_PORT_H

#include "rb_objectatomic.h"
#include "RVector.h"

/**
 * Component custom connection point or so-called port. The name port comes from
 * the piping and equipment components such as pipe, elbow and reducer. The port
 * is the opening through which the fluid can pass. A pipe has two standard
 * ports (in and out) which are part of the component itself. These ports are
 * depending the size of the component. The same is applicable for the tees,
 * elbows, etc. The RB_Port class is used where custom connections are made to
 * for example a pipe or cylinder. A nozzle can be connected to the side of a 
 * vessel where no standard port is available. A component contains a list of
 * custom connections that is refering to objects from this class 
 */
class RB_Port : public RB_ObjectAtomic {
public:
    RB_Port(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
	RB_Port(RB_Port* port);
	virtual ~RB_Port();
	
        virtual RVector location() const;
        virtual RVector direction() const;
	virtual double orientation() const;
	
        virtual void setLocation(const RVector& v);
        virtual void setDirection(const RVector& v);
	virtual void setOrientation(double angle);
	
protected:
	void createMembers();
	
};

#endif /*RB_PORT_H*/
