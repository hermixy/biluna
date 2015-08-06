/*****************************************************************
 * $Id: peng_pedclassif.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 17, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_pedclassif.h"


/**
 * Constructor
 */
PENG_PedClassif::PENG_PedClassif(const RB_String& Id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic(Id, p, n, f) {
	createMembers();
}


/**
 * Copy constructor
 */
PENG_PedClassif::PENG_PedClassif(PENG_PedClassif* classif)
                    : RB_ObjectAtomic(classif) {
	createMembers();
    *this = *classif;
}

/**
 * Destructor
 */
PENG_PedClassif::~PENG_PedClassif() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}


/**
    Create fixed members of object for the correct functioning
    of the application. The members are:
 */
void PENG_PedClassif::createMembers() {
    addMember("pedtype_id", "-", 0, RB2::MemberInteger); // PED equipment/pipe type
    addMember("assessmentby_id", "-", 0, RB2::MemberInteger);
    addMember("fluidstate_id", "-", 0, RB2::MemberInteger);
    addMember("fluidgroup_id", "-", 0, RB2::MemberInteger);
    addMember("nominalsize", "-", 0.0, RB2::MemberDouble);
    addMember("additionalinfo_id", "-", 0, RB2::MemberInteger);
    addMember("unstablegas", "-", "false", RB2::MemberChar10);

    addMember("fluid1", "-", "", RB2::MemberChar125);
    addMember("fluid2", "-", "", RB2::MemberChar125);
    addMember("fluid3", "-", "", RB2::MemberChar125);
    addMember("fluid4", "-", "", RB2::MemberChar125);

}
