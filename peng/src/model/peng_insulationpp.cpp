/*****************************************************************
 * $Id: peng_insulationpp.cpp 2165 2014-09-24 19:22:36Z rutger $
 * Created: Jul 9, 2005 3:55:29 PM - rutger
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_insulationpp.h"

PENG_InsulationPP::PENG_InsulationPP (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


PENG_InsulationPP::PENG_InsulationPP(PENG_InsulationPP* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


PENG_InsulationPP::~PENG_InsulationPP() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - linesize pipe line size
 * - thicknessplus050 insulation thickness at 50 degree Celsius (operating temperature)
 */
void PENG_InsulationPP::createMembers() {
    addMember("linesize", "NomInch", 0.0, RB2::MemberDouble);
    addMember("thicknessplus050", "mm", 0, RB2::MemberInteger);
    addMember("thicknessplus100", "mm", 0, RB2::MemberInteger);
    addMember("thicknessplus150", "mm", 0, RB2::MemberInteger);
    addMember("thicknessplus200", "mm", 0, RB2::MemberInteger);
    addMember("thicknessplus250", "mm", 0, RB2::MemberInteger);
    addMember("thicknessplus300", "mm", 0, RB2::MemberInteger);
    addMember("thicknessplus350", "mm", 0, RB2::MemberInteger);
    addMember("thicknessplus400", "mm", 0, RB2::MemberInteger);
    addMember("thicknessplus450", "mm", 0, RB2::MemberInteger);
    addMember("thicknessplus500", "mm", 0, RB2::MemberInteger);
    addMember("thicknessplus550", "mm", 0, RB2::MemberInteger);
    addMember("thicknessplus600", "mm", 0, RB2::MemberInteger);
}
