/*****************************************************************
 * $Id: peng_linecustom.cpp 2154 2014-07-30 19:44:11Z rutger $
 * Created: 2004 - rutger
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_linecustom.h"


/**
 * Constructor
 * @param id Uuid universal unique identification
 * @param p parent object container
 * @param n name of this object, usually PENG_LineCustom
 * @param f object factory interface, used for cloning and copying of object
 */
PENG_LineCustom::PENG_LineCustom (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
	
    createMembers();
}


PENG_LineCustom::PENG_LineCustom(PENG_LineCustom* line) : RB_ObjectContainer(line) {
    createMembers();
    *this = *line;
}


PENG_LineCustom::~PENG_LineCustom() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - customfield1value
 * - customfield2value
 * - customfield3value
 * - customfield4value
 * - customfield5value
 * - customfield6text
 * - customfield7text
 * - customfield8text
 * - customfield9text
 * - customfield10text
 */
void PENG_LineCustom::createMembers() {
    addMember(this, "customfield1value", "(customunit1)", 0.0, RB2::MemberDouble);
    addMember(this, "customfield2value", "(customunit2)", 0.0, RB2::MemberDouble);
    addMember(this, "customfield3value", "(customunit3)", 0.0, RB2::MemberDouble);
    addMember(this, "customfield4value", "(customunit4)", 0.0, RB2::MemberDouble);
    addMember(this, "customfield5value", "(customunit5)", 0.0, RB2::MemberDouble);
    addMember(this, "customfield6text", "-", "", RB2::MemberChar125);
    addMember(this, "customfield7text", "-", "", RB2::MemberChar125);
    addMember(this, "customfield8text", "-", "", RB2::MemberChar125);
    addMember(this, "customfield9text", "-", "", RB2::MemberChar125);
    addMember(this, "customfield10text", "-", "", RB2::MemberChar125);
}


/**
 * interception of RB_ObjectBase::setValue() to set the previousValue 
 * if previousValue was not set before with original value
 */
void PENG_LineCustom::setValue(int number, const RB_Variant& var) {
    if (!RB_ObjectBase::getPValue(number).isValid()) {
        RB_Variant mVar = RB_ObjectBase::getValue(number);

        if (mVar.isValid()) {
            RB_ObjectBase::setPValue(number, mVar);
        } else {
            // error
            RB_DEBUG->print("PENG_LineCustom::setValue(number, var) number: %i does not exist",
                number);
            return;
        }
    }

    RB_ObjectBase::setValue(number, var);
}

/**
 * interception of RB_ObjectBase::setValue() to set the previousValue 
 * if previousValue was not set before with original value
 */
void PENG_LineCustom::setValue(const RB_String& name, const RB_Variant& var) {
    if (!RB_ObjectBase::getPValue(name).isValid()) {
        RB_Variant mVar = RB_ObjectBase::getValue(name);

        if (mVar.isValid()) {
            RB_ObjectBase::setPValue(name, mVar);
        } else {
            // error
            RB_DEBUG->print("PENG_LineCustom::setValue(name, var) name: %s does not exist",
                name.toStdString().c_str());
            return;
        }
    }

    RB_ObjectBase::setValue(name, var);
}

