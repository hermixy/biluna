/*****************************************************************
 * $Id: peng_setting.cpp 2155 2014-08-04 16:50:28Z rutger $
 * Created: Aug 4, 2014 13:04:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_setting.h"


PENG_Setting::PENG_Setting (const RB_String& id, RB_ObjectBase* p,
                            const RB_String& n, RB_ObjectFactory* f)
                                : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


PENG_Setting::PENG_Setting(PENG_Setting* project) : RB_ObjectContainer(project) {
    createMembers();
    *this = *project;
}


PENG_Setting::~PENG_Setting() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}


void PENG_Setting::createMembers() {
    addMember("category_id", "-", 0, RB2::MemberInteger);
    addMember("setting_id", "-", 0, RB2::MemberInteger);
    addMember("value", "-", "<NEW>", RB2::MemberChar125);
    addMember("description", "-", "", RB2::MemberChar255);
}
