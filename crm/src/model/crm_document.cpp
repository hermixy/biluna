/*****************************************************************
 * $Id: crm_document.cpp 2076 2014-01-27 16:02:14Z rutger $
 * Created: Jan 27, 2014 14:24:25 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_document.h"

CRM_Document::CRM_Document (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


CRM_Document::CRM_Document(CRM_Document* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


CRM_Document::~CRM_Document() {
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
void CRM_Document::createMembers() {
    addMember(this, "refno", "-", 0, RB2::MemberInteger);
    addMember(this, "subject", "-", "<NEW>", RB2::MemberChar125);
    addMember(this, "customer_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "contact_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "template_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "content", "-", "", RB2::MemberString);
}
