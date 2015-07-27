/*****************************************************************
 * $Id: rb_component.h 1452 2011-09-10 08:19:50Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna GL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_COMPONENT_H
#define RB_COMPONENT_H

#include "rb_node.h"
#include "rb_objectcontainer.h"
#include "rb_port.h"
#include "RMatrix.h"

/**
 * Base class with functions of components such as cylinder, elbow, flange 
 * and head
 */
class RB_Component : public RB_ObjectContainer {
    
public:
    RB_Component(const RB_String& id = "", RB_ObjectBase* p = NULL,
                 const RB_String& n = "", RB_ObjectFactory* f = NULL);
    RB_Component(RB_Component* component);
    virtual ~RB_Component();
    
    virtual void setStandardPortCount(int nosp);
    virtual int standardPortCount();
    virtual int portCount();
    
    /**
     * first 'const' says: do not change return value, 
     * second: do not change object member values, could not be used due
     * not using const getObject() and const_iter in RB_ObjectBase?
     * TODO: check use of const_iter in RB_ObjectBase family
     */
    // const RB_Port port(int portNumber) /* const */;

    // custom ports only
    virtual RB_Port* addPort()  {
            RB_DEBUG->print("RB_Component::addPort() virtual function ERROR");
            return NULL;
    }
	
    virtual RB_Port* addPort(const RB_Port& p);
    virtual void removePort(const RB_String& portId);
    virtual void removePort(RB_Port* p);
    virtual void setPort(const RB_String& portId, const RB_Port& p);
    virtual void setPort(const RB_String& /* portId */, double /* dist */,
                         double /* rot */, const RVector& /* dir */,
                         double /* orient */)  {
        RB_DEBUG->print("RB_Component::setPort() virtual function ERROR");
    }

    virtual void port(const RB_String& portId, RB_Port& p);
    virtual void port(int portNumber, RB_Port& p);

    virtual void setNode(const RB_Node& parentNode,  const RB_String& portId,
                         RB_Node* currentNode);

    // TODO: updatePort to be protected or private?
    virtual void updatePort(const RB_String& portId,
                            RVector& localLoc, RVector& localDir);

    // x-direction
    virtual double getLength() {
        RB_DEBUG->print("RB_Component::getLength() virtual function ERROR");
        return 0.0;
    }

    // y-direction
    virtual double getWidth() {
        RB_DEBUG->print("RB_Component::getWidth() virtual function ERROR");
        return 0.0;
    }

    // z-direction
    virtual double getHeight() {
        RB_DEBUG->print("RB_Component::getHeight() virtual function ERROR");
        return 0.0;
    }

    virtual GV2::CompVisibility getVisibility();
    virtual void setVisibility(GV2::CompVisibility vis);

private:
    /** Component number of standard ports */
    int numberOfStandardPorts; // TODO: move this item to be a member
    /** Visibility flag of component, this value is not stored and only used
        to facilitate editing of equipment, piping systems or structures */
    GV2::CompVisibility visibility;

};

#endif /*RB_COMPONENT_H*/
