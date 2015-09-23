/*****************************************************************
 * $Id: peng_equipment.cpp 1983 2013-09-02 18:10:02Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_equipment.h"

//#include "rb_node.h"
#include "rb_objectcontainer.h"

PENG_Equipment::PENG_Equipment (const RB_String& Id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
					: RB_ObjectContainer (Id, p, n, f) {
	createMembers();
}


PENG_Equipment::PENG_Equipment(PENG_Equipment* equipment) 
					: RB_ObjectContainer(equipment) {
	createMembers();
	*this = *equipment;
}


PENG_Equipment::~PENG_Equipment() { 
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}


/**
    Create fixed members of object for the correct functioning
    of the application. The members are:
    number
    revision
    type_idx is an attribute of the XML file but not an element. During
      object creation it is a member of PENG_Equipment. It is for convenience
      reasons only, allowing quick browsing of lists/tables,
      example Centrifugal Pump, name is set by the user
    eqtype_id is an attribute of the XML file but not an element. During
      object creation it is a member of PENG_Equipment. It is for convenience
      reasons only, allowing quick browsing of lists/tables, The 'type and
      'eqsubtype' can be found via PENG_Type. The 'eqltype' is actually the
      type id but the word typeid might give programming problems, is system
      type. example CentrifugalPump
    eqsubtype is an attribute of the XML file but not an element. During
      object creation it is a member of PENG_Equipment. It is for convenience
      reasons only, allowing quick browsing of lists/tables, is a system type.
      example StaticMisc or static or rotating, etc.
    eqname
    field1
    field2
    field3
    designtemp
    designpress
    material
    fluid_idx
    length
    width
    height
    weight
    volume
    pnid_idx

    requisition_idx
    supplier_idx
    manufacturer
    model
    serial
    mcase
    governingcase

    svg
 */
void PENG_Equipment::createMembers() {
    addMember("number", "-", "", RB2::MemberChar40); // V-1201
    addMember("revision", "-", 0, RB2::MemberInteger);
    addMember("type_idx", "-", "0", RB2::MemberChar165); // Horizontal Drum
    addMember("eqtype_id", "-", "0", RB2::MemberChar40); // Equipment list sub Static Equipment page
    addMember("eqsubtype_id", "-", 0, RB2::MemberInteger); // Static or rotating
    
    addMember("eqname", "-", "", RB2::MemberChar40); // Holdup drum
    addMember("field1", "-", 0.0, RB2::MemberDouble);
    addMember("field2", "-", 0.0, RB2::MemberDouble);
    addMember("field3", "-", 0.0, RB2::MemberDouble);
    addMember("designtemp", "degC", 0.0, RB2::MemberDouble);
    addMember("designpress", "MPa", 0.0, RB2::MemberDouble);
    addMember("material", "-", "", RB2::MemberChar125);
    addMember("fluid_idx", "-", "0", RB2::MemberChar165);
    addMember("length", "mm", 0.0, RB2::MemberDouble);
    addMember("width", "mm", 0.0, RB2::MemberDouble);
    addMember("height", "mm", 0.0, RB2::MemberDouble);
    addMember("weight", "kg", 0.0, RB2::MemberDouble);
    addMember("volume", "litres", 0.0, RB2::MemberDouble);
    addMember("pnid_idx", "-", "0", RB2::MemberChar165);
    
    addMember("requisition_idx", "-", "0", RB2::MemberChar165);
    addMember("supplier_idx", "-", "0", RB2::MemberChar165);
    addMember("manufacturer", "-", "", RB2::MemberChar125);
    addMember("model", "-", "", RB2::MemberChar125);
    addMember("serial", "-", "", RB2::MemberChar125);
    addMember("mcase", "-", "", RB2::MemberChar125);
    addMember("governingcase", "-", "", RB2::MemberChar125);
    
    addMember("svg", "-", "", RB2::MemberString);
}

/**
 * Number of components
 * @return component count
 */
int PENG_Equipment::countComponent() {
    int listCount = countObject();
    RB_ObjectContainer* list = NULL;
    int componentCount = 0;

    for (int i = 0; i < listCount; ++i) {
        list = getContainer(i);
        RB_String ln = list->getName();

        // use only the component lists
        if (ln != "RB_NodeList" && ln != "PENG_SectionList") {
                componentCount += list->countObject();
        }
    }

    return componentCount;
}


/**
 * Number of nodes
 * @return node count
 */
int PENG_Equipment::countNode() {
    RB_ObjectContainer* list = getContainer("RB_NodeList");
    int componentCount = list->countObject();

    return componentCount;
}


/**
 * Get equipment component, such as flange, pipe
 * @param id id of component
 * @retval component or NULL if not found
 */
PENG_Component* PENG_Equipment::component(const RB_String& id) {
    int listCount = countObject();

    if (listCount < 1) {
        RB_DEBUG->print("PENG_Equipment::component() no objects in list WARNING");
        return NULL;
    }

    RB_ObjectContainer* list = NULL;
    PENG_Component* comp = NULL;

    // find component in lists except node list
    for (int i = 0; i < listCount && !comp; ++i) {
        list = getContainer(i);

        // use only the component lists
        if (list->getName() != "RB_NodeList"
                                && list->getName() != "PENG_SectionList") {
            comp = (PENG_Component*)list->getObject(id);
        }
    }

    if (listCount > 0 && !comp) {
            RB_DEBUG->print("PENG_Equipment::component() component ERROR");
            return NULL;
    }

    return comp;
}


/**
 * Get node positioning the component
 * @param comp component
 * @retval node
 */
RB_Node* PENG_Equipment::node(PENG_Component* comp) {
    if (!comp) return NULL;
    RB_ObjectContainer* list = getContainer("RB_NodeList");
    return (RB_Node*)list->getObject("connectcomponentid", comp->getId());
}


/**
 * Interception of RB_ObjectBase::setValue() to set the previousValue 
 * if previousValue was not set before with original value
 */
void PENG_Equipment::setValue(int number, const RB_Variant& var) {
    RB_Variant mVar = RB_ObjectBase::getValue(number);

    if (!mVar.isValid()) {
        // error
        RB_DEBUG->error("PENG_Equipment::setValue(number, var) number = "
                        + QString::number(number) + " does not exist ERROR");
        return;
    }

    // no change
    if (mVar == var) return;

    // value changed
    RB_Variant pVar = RB_ObjectBase::getPValue(number);

    if (!pVar.isValid()) {
        // set previous value
        RB_ObjectBase::setPValue(number, mVar);
    } else if (pVar == var) {
        // invalidate previous value
        RB_ObjectBase::setPValue(number, RB_Variant());
    }

    RB_ObjectBase::setValue(number, var);
}

/**
 * Interception of RB_ObjectBase::setValue() to set the previousValue 
 * if previousValue was not set before with original value
 */
void PENG_Equipment::setValue(const RB_String& name, const RB_Variant& var) {
    RB_Variant mVar = RB_ObjectBase::getValue(name);

    if (!mVar.isValid()) {
        // error
        RB_DEBUG->error("PENG_Equipment::setValue(number, var) name = "
                        + name + " does not exist ERROR");
        return;
    }

    // no change
    if (mVar == var) return;

    // value changed
    RB_Variant pVar = RB_ObjectBase::getPValue(name);

    if (!pVar.isValid()) {
        // set previous value
        RB_ObjectBase::setPValue(name, mVar);
    } else if (pVar == var) {
        // invalidate previous value
        RB_ObjectBase::setPValue(name, RB_Variant());
    }

    RB_ObjectBase::setValue(name, var);
}
