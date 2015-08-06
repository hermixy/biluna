/*****************************************************************
 * $Id: peng_pipeclasspt.cpp 2179 2014-09-25 14:22:50Z rutger $
 * Created: Oct 16, 2005 1:20:39 PM - rutger
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_pipeclasspt.h"


PENG_PipeClassPT::PENG_PipeClassPT(const RB_String& id, RB_ObjectBase* p,
                            const RB_String& n, RB_ObjectFactory *f)
							: RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}


PENG_PipeClassPT::PENG_PipeClassPT(PENG_PipeClassPT* pipeClassPt)
                                            : RB_ObjectAtomic(pipeClassPt) {
	createMembers();
    *this = *pipeClassPt;
}
	
	
PENG_PipeClassPT::~PENG_PipeClassPT() {
    // clean up of children is done in RB_ObjectContainer and
    // the members is done in RB_ObjectBase
}


void PENG_PipeClassPT::createMembers() {
    addMember("designtemperature", "DegC", 0.0, RB2::MemberDouble);
    addMember("designpressure", "MPa", 0.0, RB2::MemberDouble);
}
