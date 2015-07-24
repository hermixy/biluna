/*****************************************************************
 * $Id: peng_equipment.h 1826 2012-12-08 00:31:33Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_EQUIPMENT_H
#define PENG_EQUIPMENT_H

#include "peng_component.h"

class PENG_Component;
class RB_Node;

/**
 * Class that holds the general equipment data applicable for all equipment
 * such as dimensions and design conditions
 */
class PENG_Equipment : public RB_ObjectContainer {
    
public:
    PENG_Equipment(const RB_String& Id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);

    PENG_Equipment(PENG_Equipment* equipment);
    virtual ~PENG_Equipment();
    
    int countComponent();
    int countNode();

    PENG_Component* component(const RB_String& id);
    RB_Node* node(PENG_Component* comp);

    void setValue(int number, const RB_Variant& var);
    void setValue(const RB_String& name, const RB_Variant& var);
	
private:
    void createMembers();


};

#endif /*PENG_EQUIPMENT_H*/
