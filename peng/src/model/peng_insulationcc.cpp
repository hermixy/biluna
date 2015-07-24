/*****************************************************************
 * $Id: peng_insulationcc.cpp 2165 2014-09-24 19:22:36Z rutger $
 * Created: Jul 9, 2005 3:55:29 PM - rutger
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_insulationcc.h"

PENG_InsulationCC::PENG_InsulationCC (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


PENG_InsulationCC::PENG_InsulationCC(PENG_InsulationCC* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


PENG_InsulationCC::~PENG_InsulationCC() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - linesize pipe line size
 * - thicknessplus050 insulation thickness at 50 degree Celsius (operating temperature)
 */
void PENG_InsulationCC::createMembers() {
    addMember(this, "linesize", "NomInch", 0.0, RB2::MemberDouble);
    addMember(this, "thicknessplus005", "mm", 0.0, RB2::MemberInteger);
    addMember(this, "thicknessplus000", "mm", 0.0, RB2::MemberInteger);
    addMember(this, "thicknessminus005", "mm", 0.0, RB2::MemberInteger);
    addMember(this, "thicknessminus010", "mm", 0.0, RB2::MemberInteger);
    addMember(this, "thicknessminus015", "mm", 0.0, RB2::MemberInteger);
    addMember(this, "thicknessminus020", "mm", 0.0, RB2::MemberInteger);
    addMember(this, "thicknessminus025", "mm", 0.0, RB2::MemberInteger);
    addMember(this, "thicknessminus030", "mm", 0.0, RB2::MemberInteger);
    addMember(this, "thicknessminus035", "mm", 0.0, RB2::MemberInteger);
    addMember(this, "thicknessminus040", "mm", 0.0, RB2::MemberInteger);
    addMember(this, "thicknessminus045", "mm", 0.0, RB2::MemberInteger);
    addMember(this, "thicknessminus050", "mm", 0.0, RB2::MemberInteger);
}
