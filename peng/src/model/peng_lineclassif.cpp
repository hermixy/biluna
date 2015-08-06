/*****************************************************************
 * $Id: peng_lineclassif.cpp 2154 2014-07-30 19:44:11Z rutger $
 * Created: 2004 - rutger
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_lineclassif.h"


/**
 * Constructor
 * @param id Uuid universal unique identification
 * @param p parent object container
 * @param n name of this object, usually PENG_LineClassif
 * @param f object factory interface, used for cloning and copying of object
 */
PENG_LineClassif::PENG_LineClassif (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
	
    createMembers();
}


PENG_LineClassif::PENG_LineClassif(PENG_LineClassif* line) : RB_ObjectContainer(line) {
    createMembers();
    *this = *line;
}


PENG_LineClassif::~PENG_LineClassif() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - classifnomoperpress
 * - classifnomopertemp
 * - classifmaxdespress
 * - classifmaxdestemp
 * - classifmindestemp
 * - classifauthorityregulation
 * - classiffluiddangergroup_id
 * - classifgasstability_id
 * - classifregulationgroup
 * - classifregulationsubgroup
 */
void PENG_LineClassif::createMembers() {
    addMember("classifnomoperpress", "MPa", 0.0, RB2::MemberDouble);
    addMember("classifnomopertemp", "DegC", 0.0, RB2::MemberDouble);
    addMember("classifmaxdespress", "MPa", 0.0, RB2::MemberDouble);
    addMember("classifmaxdestemp", "DegC", 0.0, RB2::MemberDouble);
    addMember("classifmindestemp", "DegC", 0.0, RB2::MemberDouble);
    addMember("classifauthorityregulation", "-", "", RB2::MemberChar125);
    addMember("classiffluiddangergroup_id", "0", "", RB2::MemberChar40);
    addMember("classifgasstability_id", "0", "", RB2::MemberChar40);
    addMember("classifregulationgroup", "-", "", RB2::MemberChar125);
    addMember("classifregulationsubgroup", "-", "", RB2::MemberChar125);
}

/**
 * interception of RB_ObjectBase::setValue() to set the previousValue 
 * if previousValue was not set before with original value
 */
void PENG_LineClassif::setValue(int number, const RB_Variant& var) {
    if (!RB_ObjectBase::getPValue(number).isValid()) {
        RB_Variant mVar = RB_ObjectBase::getValue(number);

        if (mVar.isValid()) {
            RB_ObjectBase::setPValue(number, mVar);
        } else {
            // error
            RB_DEBUG->print("PENG_LineClassif::setValue(number, var) number: %i does not exist",
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
void PENG_LineClassif::setValue(const RB_String& name, const RB_Variant& var) {
    if (!RB_ObjectBase::getPValue(name).isValid()) {
        RB_Variant mVar = RB_ObjectBase::getValue(name);

        if (mVar.isValid()) {
            RB_ObjectBase::setPValue(name, mVar);
        } else {
            // error
            RB_DEBUG->print("PENG_LineClassif::setValue(name, var) name: %s does not exist",
                name.toStdString().c_str());
            return;
        }
    }

    RB_ObjectBase::setValue(name, var);
}

