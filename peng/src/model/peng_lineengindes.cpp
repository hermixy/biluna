/*****************************************************************
 * $Id: peng_lineengindes.cpp 2165 2014-09-24 19:22:36Z rutger $
 * Created: 2004 - rutger
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_lineengindes.h"


/**
 * Constructor
 * @param id Uuid universal unique identification
 * @param p parent object container
 * @param n name of this object, usually PENG_LineEnginDes
 * @param f object factory interface, used for cloning and copying of object
 */
PENG_LineEnginDes::PENG_LineEnginDes (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
	
    createMembers();
}


PENG_LineEnginDes::PENG_LineEnginDes(PENG_LineEnginDes* line) : RB_ObjectContainer(line) {
    createMembers();
    *this = *line;
}


PENG_LineEnginDes::~PENG_LineEnginDes() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - engindespnidfrom
 * - engindespnidto
 * - engindesgafrom
 * - engindesgato
 * - engindesisofrom
 * - engindesisoto
 * - engindeslinefunction_id
 * - engindesexternalpress_id
 * - engindestracingtype_id
 * - engindesnotracer
 * - engindesinsulationtype_id
 * - engindesinsulationthickness
 * - engindesusepaintspec_id true or false
 * - engindespaintsystem TODO: change to _id(x) of paintspec system
 */
void PENG_LineEnginDes::createMembers() {
    addMember(this, "engindespnidfrom", "-", "", RB2::MemberChar125);
    addMember(this, "engindespnidto", "-", "", RB2::MemberChar125);
    addMember(this, "engindesgafrom", "-", "", RB2::MemberChar125);
    addMember(this, "engindesgato", "-", "", RB2::MemberChar125);
    addMember(this, "engindesisofrom", "-", "", RB2::MemberChar125);
    addMember(this, "engindesisoto", "-", "", RB2::MemberChar125);
    addMember(this, "engindeslinefunction_id", "-", 0, RB2::MemberInteger);
    addMember(this, "engindesexternalpress_id", "-", 0, RB2::MemberInteger);
    addMember(this, "engindestracingtype_id", "-", 0, RB2::MemberInteger);
    addMember(this, "engindesnotracer", "-", 0, RB2::MemberInteger);
    addMember(this, "engindesinsulationtype_id", "-", 0, RB2::MemberInteger);
    addMember(this, "engindesinsulationthickness", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "engindesusepaintspec_id", "-", 0, RB2::MemberInteger);
    addMember(this, "engindespaintsystem", "-", "", RB2::MemberChar125);
    addMember(this, "enginstresscalcrequired_id", "-", 0, RB2::MemberInteger);
    addMember(this, "enginstresscalcnumber", "-", "", RB2::MemberChar125);
}

/**
 * interception of RB_ObjectBase::setValue() to set the previousValue 
 * if previousValue was not set before with original value
 */
void PENG_LineEnginDes::setValue(int number, const RB_Variant& var) {
    if (!RB_ObjectBase::getPValue(number).isValid()) {
        RB_Variant mVar = RB_ObjectBase::getValue(number);

        if (mVar.isValid()) {
            RB_ObjectBase::setPValue(number, mVar);
        } else {
            // error
            RB_DEBUG->print("PENG_LineEnginDes::setValue(number, var) number: %i does not exist",
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
void PENG_LineEnginDes::setValue(const RB_String& name, const RB_Variant& var) {
    if (!RB_ObjectBase::getPValue(name).isValid()) {
        RB_Variant mVar = RB_ObjectBase::getValue(name);

        if (mVar.isValid()) {
            RB_ObjectBase::setPValue(name, mVar);
        } else {
            // error
            RB_DEBUG->print("PENG_LineEnginDes::setValue(name, var) name: %s does not exist",
                name.toStdString().c_str());
            return;
        }
    }

    RB_ObjectBase::setValue(name, var);
}

