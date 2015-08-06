/*****************************************************************
 * $Id: srm_document.cpp 2073 2014-01-25 17:07:56Z rutger $
 * Created: Jan 22, 2014 14:24:25 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna SRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_document.h"

SRM_Document::SRM_Document (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


SRM_Document::SRM_Document(SRM_Document* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


SRM_Document::~SRM_Document() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/*!
    Create members. The members are (name, unit, default value, type):
    \li refno, -, 0, RB2::MemberInteger
    \li subject, -, <NEW>, RB2::MemberChar125
    \li supplier_idx, -, 0, RB2::MemberChar165
    \li contact_idx, -, 0, RB2::MemberChar165
    \li template_idx, -, 0, RB2::MemberChar165
    \li content, -, '', RB2::MemberString

 */
void SRM_Document::createMembers() {
    addMember("refno", "-", 0, RB2::MemberInteger);
    addMember("subject", "-", "<NEW>", RB2::MemberChar125);
    addMember("supplier_idx", "-", "0", RB2::MemberChar165);
    addMember("contact_idx", "-", "0", RB2::MemberChar165);
    addMember("template_idx", "-", "0", RB2::MemberChar165);
    addMember("content", "-", "", RB2::MemberString);
}
