/*****************************************************************
 * $Id: std_material.cpp 0001 2016-05-21T09:15:12 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_material.h"

STD_Material::STD_Material(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}

STD_Material::STD_Material(STD_Material* obj) : 
                   RB_ObjectContainer(obj) {
	createMembers();
	*this = *obj;
}

STD_Material::~STD_Material() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li mname material description
 * \li number material number or Werkstoff number
 * \li type_id material or form type ID: NONE, BAR, BOLTING, CASTING, FITTING, FORGING, NUT, PIPE, PLATE, ROD, TUBE
 * \li class_id material class ID: NONE, CARBON STEEL, HIGH ALLOY, LOW ALLOY, STAINLESS STEEL, NON FERRO'S
 * \li structure_id material structure ID: None, Ferritic, Austenitic, Martensitic, Austenitic-Ferritic, Non-Ferrous, other
 * \li composition chemical composition
 * \li document source document
 * \li issue source document issue/date
 * \li elasmodultable_id elasticity modulus table ID
 * \li thermexptable_id thermal expansion table ID
 * \li creeptable_id creep table ID
 * \li extpresstable_id external pressure table ID
 * \li yield20c yield strength at ambient temperature [MPa]
 * \li yield1p20c yield strength 1P at ambient temperature [MPa]
 * \li tensile20c tensile strength at ambient temperature [MPa]
 * \li specificgravity specific gravity [kg/dm3]
 * \li elongafterrupt elongation after rupture [percent]
 * \li comment comment with material
 * \li wstgruppe temp
 * \li form_en temp
 * \li tmp_id temp
 * \li tmptype_id temp ex ferritic, 0 = austenitic
 * \li tmpclass_id temp ex cast, 0 = cast
 * \li tmpelasmodultable_id temp
 * \li tmpthermexptable_id temp
 * \li tmpcreeptable_id temp
 * \li tmpextpresstable_id temp
 */
void STD_Material::createMembers() {
    addMember("mname", "-", "", RB2::MemberChar125);
    addMember("number", "-", "", RB2::MemberChar125);
    addMember("type_id", "-", 0, RB2::MemberInteger);
    addMember("class_id", "-", 0, RB2::MemberInteger);
    addMember("structure_id", "-", 0, RB2::MemberInteger);
    addMember("composition", "-", "", RB2::MemberChar125);
    addMember("document", "-", "", RB2::MemberChar125);
    addMember("issue", "-", "", RB2::MemberChar40);
    addMember("elasmodultable_id", "-", "0", RB2::MemberChar40);
    addMember("thermexptable_id", "-", "0", RB2::MemberChar40);
    addMember("creeptable_id", "-", "0", RB2::MemberChar40);
    addMember("extpresstable_id", "-", "0", RB2::MemberChar40);
    addMember("yield20c", "MPa", 0.0, RB2::MemberDouble);
    addMember("yield1p20c", "MPa", 0.0, RB2::MemberDouble);
    addMember("tensile20c", "MPa", 0.0, RB2::MemberDouble);
    addMember("specificgravity", "kg/dm3", 0.0, RB2::MemberDouble);
    addMember("elongafterrupt", "percent", 0.0, RB2::MemberDouble);
    addMember("comment", "-", "", RB2::MemberChar255);
    addMember("wstgruppe", "-", "", RB2::MemberChar125);
    addMember("form_en", "-", "", RB2::MemberChar125);
//    addMember("tmp_id", "-", "", RB2::MemberChar125);
//    addMember("tmptype_id", "-", "", RB2::MemberChar125);
//    addMember("tmpclass_id", "-", "", RB2::MemberChar125);
//    addMember("tmpelasmodultable_id", "-", "", RB2::MemberChar125);
//    addMember("tmpthermexptable_id", "-", "", RB2::MemberChar125);
//    addMember("tmpcreeptable_id", "-", "", RB2::MemberChar125);
//    addMember("tmpextpresstable_id", "-", "", RB2::MemberChar125);
}
