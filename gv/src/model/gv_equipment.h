/*****************************************************************
 * $Id: gv_equipment.h 1380 2011-03-07 20:47:51Z rutger $
 * Created: Apr 24, 2008 10:24:54 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_EQUIPMENT_H
#define GV_EQUIPMENT_H

#include "gv_component.h"
#include "gv_node.h"
#include "rb_objectcontainer.h"


/**
 * Class that holds the general equipment data applicable for all equipment
 * such as dimensions and design conditions for the information at the top or
 * bottom of the drawing, the symbol (SVG) information and port information
 */
class GV_Equipment : public RB_ObjectContainer {
    
public:
    GV_Equipment(const RB_String& Id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);

    GV_Equipment(GV_Equipment* equipment);
    virtual ~GV_Equipment();
    
    virtual int countComponent();
    virtual int countNode();

	virtual GV_Component* component(const RB_String& id);
	virtual GV_Node* node(GV_Component* comp);

	virtual void setValue(int number, const RB_Variant& var);
	virtual void setValue(const RB_String& name, const RB_Variant& var);
	
private:
	void createMembers();
	
};
	
#endif /*GV_EQUIPMENT_H*/
