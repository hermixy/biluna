/*****************************************************************
 * $Id: peng_line.cpp 2154 2014-07-30 19:44:11Z rutger $
 * Created: 2004 - rutger
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_line.h"


/**
 * Constructor
 * @param id Uuid universal unique identification
 * @param p parent object container
 * @param n name of this object, usually PENG_Line
 * @param f object factory interface, used for cloning and copying of object
 */
PENG_Line::PENG_Line (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
	
    createMembers();
    return;

    // Not used
//	if (l == RB2::DataDetailDefault) {
//		createProcessMembers();
//		createEnginDesMembers();
//		createClassifMembers();
//		createOperMaintMembers(); not
//		createRemarkMembers(); not
//		createCustomMembers();
//	}
}


PENG_Line::PENG_Line(PENG_Line* line) : RB_ObjectContainer(line) {
    createMembers();
    *this = *line;
}


PENG_Line::~PENG_Line() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - number line number (sequence number only, without pre- or postfix)
 * - revision revision number of this line, same or one up from line table revision
 * - sizedn line size in DN
 * - processutility_id process (0) or utility (1) line
 * - fromdesignation from equipment or line designation
 * - fromtype_id user equipment or line type
 * - fromsub nozzle number
 * - todesignation to equipment or line designation
 * - totype_id user equipment or line type
 * - tosub nozzle number
 * - fluid_idx fluid ID and name
 * - pipeclass_idx pipe class ID and number
 * - designtemp design temperature in degree Celsius
 * - designpress design pressure in MPa

 */
void PENG_Line::createMembers() {
    addMember("number", "-", 0, RB2::MemberInteger);
    addMember("revision", "-", 0, RB2::MemberInteger);
    addMember("sizedn", "DN", 0, RB2::MemberInteger);
    addMember("processutility_id", "-", 0, RB2::MemberInteger);
    addMember("fromdesignation", "-", "", RB2::MemberChar125);
    addMember("fromtype_id", "-", "0", RB2::MemberChar40); // user type
    addMember("fromsub", "-", "", RB2::MemberChar125); // nozzle number
    addMember("todesignation", "-", "", RB2::MemberChar125);
    addMember("totype_id", "-", "0", RB2::MemberChar40); // user type
    addMember("tosub", "-", "", RB2::MemberChar125); // nozzle number
    addMember("fluid_idx", "-", "0", RB2::MemberChar165);
    addMember("pipeclass_idx", "-", "0", RB2::MemberChar165);
    addMember("designtemp", "DegC", 0.0, RB2::MemberDouble);
    addMember("designpress", "MPa", 0.0, RB2::MemberDouble);
}

void PENG_Line::createProcessMembers() {
    addMember("processFluidDescription", "-", "", RB2::MemberChar125);
    addMember("processOperatingPress", "MPa", 0.0, RB2::MemberDouble);
    addMember("processOperatingTempStd", "DegC", 0.0, RB2::MemberDouble);
    addMember("processOperatingTempHigh", "DegC", 0.0, RB2::MemberDouble);
    addMember("processOperatingTempLow", "DegC", 0.0, RB2::MemberDouble);
    addMember("processTestMedium", "-", "", RB2::MemberChar125);
    addMember("processTestPress", "MPa", 0.0, RB2::MemberDouble);
    addMember("processPressDrop", "MPa", 0.0, RB2::MemberDouble);
    addMember("processMassFlowRateStd", "kg/s", 0.0, RB2::MemberDouble);
    addMember("processMassFlowRateMax", "kg/s", 0.0, RB2::MemberDouble);
    addMember("processVelocity", "m/s", 0.0, RB2::MemberDouble);
    addMember("processPhase", "-", "", RB2::MemberChar40);
    addMember("processBoilingPoint", "DegC", 0.0, RB2::MemberDouble);
    addMember("processVapourPress", "MPa", 0.0, RB2::MemberDouble);
    addMember("processMolWeight", "-", 0.0, RB2::MemberDouble);
    addMember("processDensityVapour", "kg/m3", 0.0, RB2::MemberDouble);
    addMember("processDensityLiquid", "kg/m3", 0.0, RB2::MemberDouble);
    addMember("processViscosityVapour", "cP", 0.0, RB2::MemberDouble);
    addMember("processViscosityLiquid", "cP", 0.0, RB2::MemberDouble);
    addMember("processStandardVolumeFlowStd", "m3/h", 0.0, RB2::MemberDouble);
    addMember("processStandardVolumeFlowMax", "m3/h", 0.0, RB2::MemberDouble);
    addMember("processActualFlowStd", "m3/h", 0.0, RB2::MemberDouble);
    addMember("processActualFlowMax", "m3/h", 0.0, RB2::MemberDouble);
    addMember("processSimulation", "-", "", RB2::MemberChar125);
}
 
void PENG_Line::createEnginDesMembers() {
    addMember("enginDesPnidFrom", "-", "", RB2::MemberChar125);
    addMember("enginDesPnidTo", "-", "", RB2::MemberChar125);
    addMember("enginDesGaFrom", "-", "", RB2::MemberChar125);
    addMember("enginDesGaTo", "-", "", RB2::MemberChar125);
    addMember("enginDesIsoFrom", "-", "", RB2::MemberChar125);
    addMember("enginDesIsoTo", "-", "", RB2::MemberChar125);
    addMember("enginDesLineFunction", "-", "", RB2::MemberChar40);
    addMember("enginDesExternalPress", "-", "", RB2::MemberChar40);
    addMember("enginDesTracingType", "-", "", RB2::MemberChar40);
    addMember("enginDesNoTracer", "-", 0, RB2::MemberInteger);
    addMember("enginDesInsulationType", "-", "", RB2::MemberChar40);
    addMember("enginDesInsulationThickness", "mm", 0, RB2::MemberInteger);
    addMember("enginDesUsePaintSpec", "-", "false", RB2::MemberChar40);
    addMember("enginDesPaintSystem", "-", "", RB2::MemberChar125);
}

void PENG_Line::createClassifMembers() {
    addMember("classifNomOperPress", "MPa", 0.0, RB2::MemberDouble);
    addMember("classifNomOperTemp", "DegC", 0.0, RB2::MemberDouble);
    addMember("classifMaxDesPress", "MPa", 0.0, RB2::MemberDouble);
    addMember("classifMaxDesTemp", "DegC", 0.0, RB2::MemberDouble);
    addMember("classifMinDesTemp", "DegC", 0.0, RB2::MemberDouble);
    addMember("classifAuthorityRegulation", "-", "", RB2::MemberChar125);
    addMember("classifFluidDangerGroup", "-", "", RB2::MemberChar40);
    addMember("classifGasStability", "-", "", RB2::MemberChar40);
    addMember("classifRegulationGroup", "-", "", RB2::MemberChar125);
    addMember("classifRegulationSubGroup", "-", "", RB2::MemberChar125);
}

void PENG_Line::createOperMaintMembers() {
    addMember("operMaintInstallDate", "-", "1901-01-01", RB2::MemberDateTime);
    addMember("operMaintOutOfService", "-", "false", RB2::MemberChar40);
    addMember("operMaintPeriodicInspection", "-", "false", RB2::MemberChar40);
    addMember("operMaintInspectionLoopNr", "-", "", RB2::MemberChar40);
    addMember("operMaintInspectionFrequency", "day", 0, RB2::MemberDateTime);
}

void PENG_Line::createRemarkMembers() {
    addMember("remarkProcess", "-", "", RB2::MemberChar255);
    addMember("remarkEnginDes", "-", "", RB2::MemberChar255);
    addMember("remarkClassification", "-", "", RB2::MemberChar255);
    addMember("remarkOperMaint", "-", "", RB2::MemberChar255);
    addMember("remarkCustom", "-", "", RB2::MemberChar255);
}
 
void PENG_Line::createCustomMembers() {
    addMember("customField1Value", "(customUnit1)", 0.0, RB2::MemberDouble);
    addMember("customField2Value", "(customUnit2)", 0.0, RB2::MemberDouble);
    addMember("customField3Value", "(customUnit3)", 0.0, RB2::MemberDouble);
    addMember("customField4Value", "(customUnit4)", 0.0, RB2::MemberDouble);
    addMember("customField5Value", "(customUnit5)", 0.0, RB2::MemberDouble);
    addMember("customField6Text", "-", "", RB2::MemberChar125);
    addMember("customField7Text", "-", "", RB2::MemberChar125);
    addMember("customField8Text", "-", "", RB2::MemberChar125);
    addMember("customField9Text", "-", "", RB2::MemberChar125);
    addMember("customField10Text", "-", "", RB2::MemberChar125);
}


/**
 * interception of RB_ObjectBase::setValue() to set the previousValue 
 * if previousValue was not set before with original value
 */
void PENG_Line::setValue(int number, const RB_Variant& var) {
    if (!RB_ObjectBase::getPValue(number).isValid()) {
        RB_Variant mVar = RB_ObjectBase::getValue(number);

        if (mVar.isValid()) {
            RB_ObjectBase::setPValue(number, mVar);
        } else {
            // error
            RB_DEBUG->print("PENG_Line::setValue(number, var) number: %i does not exist",
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
void PENG_Line::setValue(const RB_String& name, const RB_Variant& var) {
    if (!RB_ObjectBase::getPValue(name).isValid()) {
        RB_Variant mVar = RB_ObjectBase::getValue(name);

        if (mVar.isValid()) {
            RB_ObjectBase::setPValue(name, mVar);
        } else {
            // error
            RB_DEBUG->print("PENG_Line::setValue(name, var) name: %s does not exist",
                name.toStdString().c_str());
            return;
        }
    }

    RB_ObjectBase::setValue(name, var);
}

