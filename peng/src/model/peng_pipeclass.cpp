/*****************************************************************
 * $Id: peng_pipeclass.cpp 2147 2014-06-04 13:19:44Z rutger $
 * Created 2005 Rutger
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_pipeclass.h"


PENG_PipeClass::PENG_PipeClass(const RB_String& id, RB_ObjectBase* p,
                               const RB_String& n, RB_ObjectFactory* f)
                : RB_ObjectContainer (id, p, n, f) {
	createMembers();
}

PENG_PipeClass::PENG_PipeClass(PENG_PipeClass* pipeClass)
                : RB_ObjectContainer(pipeClass) {
	createMembers();
	*this = *pipeClass;	
}
	
PENG_PipeClass::~PENG_PipeClass() {
	// clean up of children is done in RB_ObjectContainer and 
	// the members is done in RB_ObjectBase
}

void PENG_PipeClass::createMembers() {
    addMember(this, "classnumber", "-", "<NEW>", RB2::MemberChar125);
    addMember(this, "revision", "-", 0, RB2::MemberInteger);
    addMember(this, "service", "-", "", RB2::MemberChar125);
    addMember(this, "rating", "-", "", RB2::MemberChar125);
    addMember(this, "materialgroup", "-", "", RB2::MemberChar125);
}
