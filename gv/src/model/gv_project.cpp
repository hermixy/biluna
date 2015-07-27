/*****************************************************************
 * $Id: gv_project.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Apr 24, 2008 9:57:06 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_project.h"

#include "gv_component.h"


GV_Project::GV_Project (const RB_String& id, RB_ObjectBase* p,
                        const RB_String& n, RB_ObjectFactory* f)
						: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}


GV_Project::GV_Project(GV_Project* project) : RB_ObjectContainer(project) {
	createMembers();
	*this = *project;
}


GV_Project::~GV_Project () {
	// deletion of children and members is done in RB_ObjectBase
	// and RB_ObjectContainer
}


void GV_Project::createMembers() {
	addMember(this, "number", "-", "New Project", RB2::MemberChar125);	// 0
    addMember(this, "revision", "-", 0,  RB2::MemberInteger);							// 1
    addMember(this, "description", "-", "", RB2::MemberChar125);
    addMember(this, "customer", "-", "", RB2::MemberChar125);
    addMember(this, "location", "-", "", RB2::MemberChar125);
}

/**
 * Synchronize the Qt graphic item data with the GV object data before saving
 *
void GV_Project::synchronizeModelData(RB_ObjectBase* obj) {
	RB_DEBUG->print("GV_Project::synchronizeModelData()");	
	
	if (!obj) {
		obj = this;
	}
	
    // write children if children are atomic or container
	if (obj->isContainer()) {    
		RB_ObjectContainer* tmpObjc = (RB_ObjectContainer*)obj;
		RB_ObjectIterator* iter = tmpObjc->createIterator();
		
		for (iter->first(); !iter->isDone(); iter->next()) {
			RB_ObjectBase* tmpObj = iter->currentObject();
			
			if (tmpObj->getName() == "GV_Line" 
							|| tmpObj->getName() == "GV_Symbol") {
				GV_Component* comp = (GV_Component*)tmpObj;
				comp->synchronizeObjectData();
			}
		}
		
		delete iter;
	}
}
*/
