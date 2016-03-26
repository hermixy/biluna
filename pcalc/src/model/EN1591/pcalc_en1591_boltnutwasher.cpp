/*****************************************************************
 * $Id: pcalc_component.cpp 1826 2012-12-08 00:31:33Z rutger $
 * Created: Apr 7, 2015 - rutger
 *
 * Copyright (C) 2015 Biluna. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_en1591_boltnutwasher.h"


PCALC_EN1591_BoltNutWasher::PCALC_EN1591_BoltNutWasher(const RB_String& id,
                                           RB_ObjectBase* p,
                                           const RB_String& n,
                                           RB_ObjectFactory* f)
                                        : RB_ObjectAtomic(id, p, n, f) {
	 createMembers();
}

PCALC_EN1591_BoltNutWasher::PCALC_EN1591_BoltNutWasher(
        PCALC_EN1591_BoltNutWasher* component)
            : RB_ObjectAtomic(component) {
	createMembers();
	*this = *component;
}

PCALC_EN1591_BoltNutWasher::~PCALC_EN1591_BoltNutWasher() {
    // nothing
}

/**
 * Create members:
 * \li -
 */
void PCALC_EN1591_BoltNutWasher::createMembers() {
    // bolt
    addMember("size_idx", "-", "0", RB2::MemberChar165);
    addMember("bolttype_id", "-", 0, RB2::MemberInteger);
    addMember("tensionertype_id", "-", 0, RB2::MemberInteger);
    addMember("db0", "mm", 0.0, RB2::MemberDouble);
    addMember("db2", "mm", 0.0, RB2::MemberDouble);
    addMember("db4", "mm", 0.0, RB2::MemberDouble);
    addMember("dbe", "mm", 0.0, RB2::MemberDouble);
    addMember("dbs", "mm", 0.0, RB2::MemberDouble);
    addMember("dn", "mm", 0.0, RB2::MemberDouble);
    addMember("en", "mm", 0.0, RB2::MemberDouble);
    addMember("l5t", "mm", 0.0, RB2::MemberDouble);
    addMember("ls", "mm", 0.0, RB2::MemberDouble);
    addMember("mun", "-", 0.0, RB2::MemberDouble);
    addMember("mut", "-", 0.0, RB2::MemberDouble);
    addMember("pt", "-", 0.0, RB2::MemberDouble);
    addMember("ruptureelongationa", "percent", 0.0, RB2::MemberDouble);
    addMember("materialbolt_idx", "-", "0", RB2::MemberChar165);
    // bolthole
    addMember("d5", "mm", 0.0, RB2::MemberDouble);
    addMember("d5t", "mm", 0.0, RB2::MemberDouble);
//    addMember("efb", "mm", 0.0, RB2::MemberDouble);
    addMember("blindhole", "-", 0, RB2::MemberInteger);
    // washer
    addMember("ew", "mm", 0.0, RB2::MemberDouble);
    addMember("dw1", "mm", 0.0, RB2::MemberDouble);
    addMember("dw2", "mm", 0.0, RB2::MemberDouble);
    addMember("materialwasher_idx", "-", "0", RB2::MemberChar165);
}
