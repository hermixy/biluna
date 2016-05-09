/*****************************************************************
 * $Id: pcalc_generatorformula.cpp 0001 2016-05-09T08:49:41 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "pcalc_generatorformula.h"

PCALC_GeneratorFormula::PCALC_GeneratorFormula(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

PCALC_GeneratorFormula::PCALC_GeneratorFormula(PCALC_GeneratorFormula* obj) : 
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

PCALC_GeneratorFormula::~PCALC_GeneratorFormula() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li number Formula number
 * \li resultname Result variable name
 * \li formulanames Formula with variable names
 * \li formulavalues Formula with variable test values
 * \li resultvalue Result of formula with test values
 * \li description Description of the result variable
 * \li validationtext Validation text, if applicable (only when a variable is validated)
 */
void PCALC_GeneratorFormula::createMembers() {
    addMember("number", "-", "", RB2::MemberChar125);
    addMember("resultname", "-", "", RB2::MemberChar125);
    addMember("formulanames", "-", "", RB2::MemberChar2500);
    addMember("formulavalues", "-", "", RB2::MemberChar2500);
    addMember("resultvalue", "-", 0.0, RB2::MemberDouble);
    addMember("description", "-", "", RB2::MemberChar255);
    addMember("validationtext", "-", "", RB2::MemberChar255);
}
