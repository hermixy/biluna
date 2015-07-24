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
    addMember(this, "number", "-", 0, RB2::MemberInteger);
    addMember(this, "revision", "-", 0, RB2::MemberInteger);
    addMember(this, "sizedn", "DN", 0, RB2::MemberInteger);
    addMember(this, "processutility_id", "-", 0, RB2::MemberInteger);
    addMember(this, "fromdesignation", "-", "", RB2::MemberChar125);
    addMember(this, "fromtype_id", "-", "0", RB2::MemberChar40); // user type
    addMember(this, "fromsub", "-", "", RB2::MemberChar125); // nozzle number
    addMember(this, "todesignation", "-", "", RB2::MemberChar125);
    addMember(this, "totype_id", "-", "0", RB2::MemberChar40); // user type
    addMember(this, "tosub", "-", "", RB2::MemberChar125); // nozzle number
    addMember(this, "fluid_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "pipeclass_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "designtemp", "DegC", 0.0, RB2::MemberDouble);
    addMember(this, "designpress", "MPa", 0.0, RB2::MemberDouble);
}

void PENG_Line::createProcessMembers() {
    addMember(this, "processFluidDescription", "-", "", RB2::MemberChar125);
    addMember(this, "processOperatingPress", "MPa", 0.0, RB2::MemberDouble);
    addMember(this, "processOperatingTempStd", "DegC", 0.0, RB2::MemberDouble);
    addMember(this, "processOperatingTempHigh", "DegC", 0.0, RB2::MemberDouble);
    addMember(this, "processOperatingTempLow", "DegC", 0.0, RB2::MemberDouble);
    addMember(this, "processTestMedium", "-", "", RB2::MemberChar125);
    addMember(this, "processTestPress", "MPa", 0.0, RB2::MemberDouble);
    addMember(this, "processPressDrop", "MPa", 0.0, RB2::MemberDouble);
    addMember(this, "processMassFlowRateStd", "kg/s", 0.0, RB2::MemberDouble);
    addMember(this, "processMassFlowRateMax", "kg/s", 0.0, RB2::MemberDouble);
    addMember(this, "processVelocity", "m/s", 0.0, RB2::MemberDouble);
    addMember(this, "processPhase", "-", "", RB2::MemberChar40);
    addMember(this, "processBoilingPoint", "DegC", 0.0, RB2::MemberDouble);
    addMember(this, "processVapourPress", "MPa", 0.0, RB2::MemberDouble);
    addMember(this, "processMolWeight", "-", 0.0, RB2::MemberDouble);
    addMember(this, "processDensityVapour", "kg/m3", 0.0, RB2::MemberDouble);
    addMember(this, "processDensityLiquid", "kg/m3", 0.0, RB2::MemberDouble);
    addMember(this, "processViscosityVapour", "cP", 0.0, RB2::MemberDouble);
    addMember(this, "processViscosityLiquid", "cP", 0.0, RB2::MemberDouble);
    addMember(this, "processStandardVolumeFlowStd", "m3/h", 0.0, RB2::MemberDouble);
    addMember(this, "processStandardVolumeFlowMax", "m3/h", 0.0, RB2::MemberDouble);
    addMember(this, "processActualFlowStd", "m3/h", 0.0, RB2::MemberDouble);
    addMember(this, "processActualFlowMax", "m3/h", 0.0, RB2::MemberDouble);
    addMember(this, "processSimulation", "-", "", RB2::MemberChar125);
}
 
void PENG_Line::createEnginDesMembers() {
    addMember(this, "enginDesPnidFrom", "-", "", RB2::MemberChar125);
    addMember(this, "enginDesPnidTo", "-", "", RB2::MemberChar125);
    addMember(this, "enginDesGaFrom", "-", "", RB2::MemberChar125);
    addMember(this, "enginDesGaTo", "-", "", RB2::MemberChar125);
    addMember(this, "enginDesIsoFrom", "-", "", RB2::MemberChar125);
    addMember(this, "enginDesIsoTo", "-", "", RB2::MemberChar125);
    addMember(this, "enginDesLineFunction", "-", "", RB2::MemberChar40);
    addMember(this, "enginDesExternalPress", "-", "", RB2::MemberChar40);
    addMember(this, "enginDesTracingType", "-", "", RB2::MemberChar40);
    addMember(this, "enginDesNoTracer", "-", 0, RB2::MemberInteger);
    addMember(this, "enginDesInsulationType", "-", "", RB2::MemberChar40);
    addMember(this, "enginDesInsulationThickness", "mm", 0, RB2::MemberInteger);
    addMember(this, "enginDesUsePaintSpec", "-", "false", RB2::MemberChar40);
    addMember(this, "enginDesPaintSystem", "-", "", RB2::MemberChar125);
}

void PENG_Line::createClassifMembers() {
    addMember(this, "classifNomOperPress", "MPa", 0.0, RB2::MemberDouble);
    addMember(this, "classifNomOperTemp", "DegC", 0.0, RB2::MemberDouble);
    addMember(this, "classifMaxDesPress", "MPa", 0.0, RB2::MemberDouble);
    addMember(this, "classifMaxDesTemp", "DegC", 0.0, RB2::MemberDouble);
    addMember(this, "classifMinDesTemp", "DegC", 0.0, RB2::MemberDouble);
    addMember(this, "classifAuthorityRegulation", "-", "", RB2::MemberChar125);
    addMember(this, "classifFluidDangerGroup", "-", "", RB2::MemberChar40);
    addMember(this, "classifGasStability", "-", "", RB2::MemberChar40);
    addMember(this, "classifRegulationGroup", "-", "", RB2::MemberChar125);
    addMember(this, "classifRegulationSubGroup", "-", "", RB2::MemberChar125);
}

void PENG_Line::createOperMaintMembers() {
    addMember(this, "operMaintInstallDate", "-", "1901-01-01", RB2::MemberDateTime);
    addMember(this, "operMaintOutOfService", "-", "false", RB2::MemberChar40);
    addMember(this, "operMaintPeriodicInspection", "-", "false", RB2::MemberChar40);
    addMember(this, "operMaintInspectionLoopNr", "-", "", RB2::MemberChar40);
    addMember(this, "operMaintInspectionFrequency", "day", 0, RB2::MemberDateTime);
}

void PENG_Line::createRemarkMembers() {
    addMember(this, "remarkProcess", "-", "", RB2::MemberChar255);
    addMember(this, "remarkEnginDes", "-", "", RB2::MemberChar255);
    addMember(this, "remarkClassification", "-", "", RB2::MemberChar255);
    addMember(this, "remarkOperMaint", "-", "", RB2::MemberChar255);
    addMember(this, "remarkCustom", "-", "", RB2::MemberChar255);
}
 
void PENG_Line::createCustomMembers() {
    addMember(this, "customField1Value", "(customUnit1)", 0.0, RB2::MemberDouble);
    addMember(this, "customField2Value", "(customUnit2)", 0.0, RB2::MemberDouble);
    addMember(this, "customField3Value", "(customUnit3)", 0.0, RB2::MemberDouble);
    addMember(this, "customField4Value", "(customUnit4)", 0.0, RB2::MemberDouble);
    addMember(this, "customField5Value", "(customUnit5)", 0.0, RB2::MemberDouble);
    addMember(this, "customField6Text", "-", "", RB2::MemberChar125);
    addMember(this, "customField7Text", "-", "", RB2::MemberChar125);
    addMember(this, "customField8Text", "-", "", RB2::MemberChar125);
    addMember(this, "customField9Text", "-", "", RB2::MemberChar125);
    addMember(this, "customField10Text", "-", "", RB2::MemberChar125);
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

