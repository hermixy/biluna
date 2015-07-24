/*****************************************************************
 * $Id: crm_template.cpp 2067 2014-01-21 20:10:28Z rutger $
 * Created: Mar 06, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_template.h"

CRM_Template::CRM_Template (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


CRM_Template::CRM_Template(CRM_Template* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


CRM_Template::~CRM_Template() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/*!
    Create members. The members are (name, unit, default value, type):
    \li type_id, -, 0, RB2::MemberInteger
    \li reference, -, '', RB2::MemberChar40
    \li default, -, '', RB2::MemberInteger
    \li description, -, <NEW>, RB2::MemberChar125
    \li lastused, -, 1900-01-01, RB2::MemberChar20
    \li standard, -. 0, RB2::MemberInteger
    \li pageheader, -, '', RB2::MemberString
    \li doccontent, -, '', RB2::MemberString
    \li pagefooter, -, '', RB2::MemberString
    \li marginleft, mm, 15.0, RB2::MemberDouble
    \li marginright, mm, 10.0, RB2::MemberDouble
    \li margintop, mm, 10.0, RB2::MemberDouble
    \li marginbottom, mm, 10.0, RB2::MemberDouble
    \li pageheaderheight, mm, 15.0, RB2::MemberString
    \li pagefooterheight, mm, 10, RB2::MemberString

 */
void CRM_Template::createMembers() {
    addMember(this, "type_id", "-", 0, RB2::MemberInteger);
    addMember(this, "reference", "-", "", RB2::MemberChar40);
    addMember(this, "standard", "-", 0, RB2::MemberInteger);
    addMember(this, "description", "-", "<NEW>", RB2::MemberChar125);
    addMember(this, "lastused", "-", "1900-01-01", RB2::MemberChar20);

    addMember(this, "pageheader", "-", "", RB2::MemberString);
    addMember(this, "doccontent", "-", "", RB2::MemberString);
    addMember(this, "pagefooter", "-", "", RB2::MemberString);

    addMember(this, "papersize", "-", 0, RB2::MemberInteger);

    addMember(this, "marginleft", "mm", 15.0, RB2::MemberDouble);
    addMember(this, "marginright", "mm", 10.0, RB2::MemberDouble);
    addMember(this, "margintop", "mm", 10.0, RB2::MemberDouble);
    addMember(this, "marginbottom", "mm", 10.0, RB2::MemberDouble);

    addMember(this, "pageheaderheight", "mm", 20.0, RB2::MemberDouble);
    addMember(this, "pagefooterheight", "mm", 20.0, RB2::MemberDouble);
}
