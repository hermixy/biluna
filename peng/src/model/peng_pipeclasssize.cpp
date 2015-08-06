/*****************************************************************
 * $Id: peng_pipeclasssize.cpp 2179 2014-09-25 14:22:50Z rutger $
 * Created: Oct 16, 2005 1:20:39 PM - rutger
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_pipeclasssize.h"


PENG_PipeClassSize::PENG_PipeClassSize(const RB_String& id, RB_ObjectBase* p,
                            const RB_String& n, RB_ObjectFactory* f)
							: RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}


PENG_PipeClassSize::PENG_PipeClassSize(PENG_PipeClassSize* pipeClassSize)
											: RB_ObjectAtomic(pipeClassSize) {
	createMembers();
	*this = *pipeClassSize;
}
	
	
PENG_PipeClassSize::~PENG_PipeClassSize() {
	// clean up of children is done in RB_ObjectBase and RB_ObjectContainer
}


void PENG_PipeClassSize::createMembers() {
    addMember("size", "NomInch", 0.0, RB2::MemberDouble);
}
