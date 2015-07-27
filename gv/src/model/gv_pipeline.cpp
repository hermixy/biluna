/*****************************************************************
 * $Id: gv_pipeline.cpp 1380 2011-03-07 20:47:51Z rutger $
 * Created: Apr 24, 2008 10:50:48 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_pipeline.h"

GV_PipeLine::GV_PipeLine (const RB_String& Id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
					: LDT_Line (Id, p, n, f, RB2::DataDetailBasic) {
//	createMembers();
}


GV_PipeLine::GV_PipeLine(GV_PipeLine* pipeLine) 
					: LDT_Line(pipeLine) {
	createMembers();
	*this = *pipeLine;
}


GV_PipeLine::~GV_PipeLine() { 
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}


/**
 * Create fixed members of object for the correct functioning of the application
 */
void GV_PipeLine::createMembers() {
//	RB_String uuid = RB_Uuid::createUuid().toString();
	RB_String uuid = "";
    addMember(uuid, this, "number", "-", "10001", RB2::MemberChar40);		// 0
//	uuid = RB_Uuid::createUuid().toString();
    addMember(uuid, this, "revision", "-", 0, RB2::MemberInteger);			// 1
    
    /*
     * 'type' is an attribute of the XML file but not an element. During 
     * object creation it is a member of GV_PipeLine. It is for convenience 
     * reasons only, allowing quick browsing of lists/tables, 
     * example Centrifugal Pump 
     */ 
    addMember(uuid, this, "type", "-", "", RB2::MemberChar40);				// 2
    /*
     * 'eqlType' is an attribute of the XML file but not an element. During 
     * object creation it is a member of GV_PipeLine. It is for convenience 
     * reasons only, allowing quick browsing of lists/tables, The 'type and 
     * 'statRot' can be found via GV_Type. The 'eqlType' is actually the 
     * type id but the word typeid might give programming problems, 
     * example CentrifugalPump 
     */
    addMember(uuid, this, "eqlType", "-", "", RB2::MemberChar40);			// 3
    /*
     * 'statRot' is an attribute of the XML file but not an element. During 
     * object creation it is a member of GV_PipeLine. It is for convenience 
     * reasons only, allowing quick browsing of lists/tables,
     * example StaticMisc 
     */ 
    addMember(uuid, this, "eqlStatRot", "-", "", RB2::MemberChar40);		// 4
    
    addMember(uuid, this, "name", "-", "", RB2::MemberChar40);				// 5
    addMember(uuid, this, "field1", "-", 0.0, RB2::MemberDouble);			// 6
    addMember(uuid, this, "field2", "-", 0.0, RB2::MemberDouble);			// 7
    addMember(uuid, this, "field3", "-", 0.0, RB2::MemberDouble);			// 8
    addMember(uuid, this, "designTemp", "DegC", 0.0, RB2::MemberDouble);	// 9
    addMember(uuid, this, "designPress", "MPa", 0.0, RB2::MemberDouble);	// 10
    addMember(uuid, this, "material", "-", "", RB2::MemberChar125);			// 11
    addMember(uuid, this, "fluid", "-", "", RB2::MemberChar125);			// 12
    addMember(uuid, this, "length", "mm", 0.0, RB2::MemberDouble);			// 13
    addMember(uuid, this, "width", "mm", 0.0, RB2::MemberDouble);			// 14
    addMember(uuid, this, "height", "mm", 0.0, RB2::MemberDouble);			// 15
    addMember(uuid, this, "weight", "kg", 0.0, RB2::MemberDouble);			// 16
    addMember(uuid, this, "pnid", "-", "", RB2::MemberChar125);				// 17
    
    addMember(uuid, this, "requisition", "-", "", RB2::MemberChar125);		// 18
    addMember(uuid, this, "supplier", "-", "", RB2::MemberChar125);			// 19
    addMember(uuid, this, "manufacturer", "-", "", RB2::MemberChar125);		// 20
    addMember(uuid, this, "model", "-", "", RB2::MemberChar125);			// 21
    addMember(uuid, this, "serial", "-", "", RB2::MemberChar125);			// 22
    addMember(uuid, this, "case", "-", "", RB2::MemberChar125);				// 23
    addMember(uuid, this, "governingCase", "-", "", RB2::MemberChar125);	// 24
    
    addMember(uuid, this, "svg", "-", "", RB2::MemberString);				// 25
    
}

/**
 * Number of components
 * @return component count = 1 only base symbol, more are nozzles 
 * /
int GV_PipeLine::countComponent() {
	int listCount = countObject();
	RB_ObjectContainer* list = NULL;
	int componentCount = 0;
	
	for (int i = 0; i < listCount; ++i) {
		list = getContainer(i);
		RB_String ln = list->getName(); 
		
		// use only the component lists
		if (ln != "GV_NodeList" && ln != "GV_SectionList") {
			componentCount += list->countObject();
		}
	}
	
	return componentCount;
}


/ **
 * Number of nodes
 * @return node count
 * /
int GV_PipeLine::countNode() {
	RB_ObjectContainer* list = getContainer("GV_NodeList");
	int componentCount = list->countObject();
	
	return componentCount;
}


/ **
 * Get equipment component, such as flange, pipe
 * @param id id of component
 * @retval component or NULL if not found
 * /
GV_Component* GV_PipeLine::component(const RB_String& id) {
	int listCount = countObject(); 
	
	if (listCount < 1) { 
		RB_DEBUG->print("GV_PipeLine::component() no objects in list WARNING");
		return NULL;
	}
	
	RB_ObjectContainer* list = NULL;
	GV_Component* comp = NULL;

	// find component in lists except node list
	for (int i = 0; i < listCount && !comp; ++i) {
		list = getContainer(i);
		
		// use only the component lists
		if (list->getName() != "GV_NodeList" 
							&& list->getName() != "GV_SectionList") {
			comp = (GV_Component*)list->getObject(id);
		}
	}
	
	if (listCount > 0 && !comp) {
		RB_DEBUG->print("GV_PipeLine::component() component ERROR");
		return NULL;
	}
	
	return comp;	
}


/ **
 * Get node positioning the component
 * @param comp component
 * @retval node
 * /
GV_Node* GV_PipeLine::node(GV_Component* comp) {
	if (!comp) return NULL;
	RB_ObjectContainer* list = getContainer("GV_NodeList");
	return (GV_Node*)list->getObject("connectId", comp->getId());
}


/ **
 * Interception of RB_ObjectBase::setValue() to set the previousValue 
 * if previousValue was not set before with original value
 * /
void GV_PipeLine::setValue(int number, const RB_Variant& var) {
	RB_Variant mVar = RB_ObjectBase::getValue(number);
	
	if (!mVar.isValid()) {
		// error
		RB_DEBUG->print("GV_PipeLine::setValue(number, var) number = %i " 
							"does not exist", number);
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

/ **
 * Interception of RB_ObjectBase::setValue() to set the previousValue 
 * if previousValue was not set before with original value
 * /
void GV_PipeLine::setValue(const RB_String& name, const RB_Variant& var) {
	RB_Variant mVar = RB_ObjectBase::getValue(name);
	
	if (!mVar.isValid()) {
		// error
		RB_DEBUG->print("GV_PipeLine::setValue(number, var) name = %s " 
								"does not exist", name.toStdString().c_str());
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
*/
