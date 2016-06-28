/*****************************************************************
 * $Id: std_enmaterialhandler.cpp 0001 2016-06-24 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_enmaterialhandler.h"
#include "pcalc.h"
#include "pcalc_modelfactory.h"
#include "pcalc_objectfactory.h"
#include "pcalc_report.h"


STD_EnMaterialHandler::STD_EnMaterialHandler()
                : Biluna::Calc::RB_TableMath(), STD_MaterialHandler() {
    mMaterialList = new RB_ObjectContainer(
                "", nullptr, "STD_MaterialList", PCALC_OBJECTFACTORY);
    mCurrentMaterial = nullptr;
    mCurrentMatName = "";
    mElasModulTableList = new RB_ObjectContainer(
                "", nullptr, "STD_ElasModulTableList", PCALC_OBJECTFACTORY);
    mCurrentElasModulTable = nullptr;
    mThermExpTableList = new RB_ObjectContainer(
                "", nullptr, "STD_ThermExpTableList", PCALC_OBJECTFACTORY);
    mCurrentThermExpTable = nullptr;
    // external pressure
}

STD_EnMaterialHandler::~STD_EnMaterialHandler() {
    delete mMaterialList;
    mMaterialList = nullptr;
    delete mElasModulTableList;
    mElasModulTableList = nullptr;
    delete mThermExpTableList;
    mThermExpTableList = nullptr;
    // external pressure
}

bool STD_EnMaterialHandler::setCurrentMaterial(const QString& materialIdx) {
    bool success = false;
    QString materialId = RB2::IdxId(materialIdx);

    mCurrentMaterial = mMaterialList->getObject(materialId);

    if (mCurrentMaterial) {
        success = true;
        mCurrentMatName = mCurrentMaterial->getDValue("mname").toString();
    } else {
        // will also set mCurrentMaterial
        success = loadMaterial(materialId);

        if (success) {
            mCurrentMatName = mCurrentMaterial->getDValue("mname").toString();
            QString id = mCurrentMaterial->getValue("elasmodultable_id").toString();
            loadElasModulTable(id);
            id = mCurrentMaterial->getValue("thermexptable_id").toString();
            loadThemExpTable(id);
            // external pressure chart
        } else {
            mCurrentMatName = "";
            RB_DEBUG->warning("STD_EnMaterialHandler::setCurrentMaterial() "
                              "material load WARNING");
        }
    }

    return success;
}

/**
 * @brief STD_EnMaterialHandler::isValid
 * @return true if valid material object
 */
bool STD_EnMaterialHandler::isValid() {
    return mCurrentMaterial != nullptr;
}

void STD_EnMaterialHandler::refresh() {
    mMaterialList->erase();
    mCurrentMaterial = nullptr;
    mCurrentMatName = "";
    mElasModulTableList->erase();
    mCurrentElasModulTable = nullptr;
    mThermExpTableList->erase();
    mCurrentThermExpTable = nullptr;
    // TODO: external pressure
}

double STD_EnMaterialHandler::allowableDesignStress(
                    double designTemp, STD2::CompType compType,
                    int loadCaseNo, const QString& variableName) {
    STD2::MatStruct matStruct = getMaterialStructure();
    double elongPercent = getElongationPercent();
    bool alternativeRoute = false; // clause 6.3
    double allowStress = 0.0;

    double Rp02 = 0.0;
    double Rp10 = 0.0;
    double RmMin = 0.0;

    if (matStruct != STD2::MatStructAustenitic
            && matStruct != STD2::MatStructCasting
            && elongPercent < 30.0
            && !alternativeRoute) {
        Rp02 = get_Rp02(designTemp, loadCaseNo);
        RmMin = get_RmMin(20, loadCaseNo); // 20 Celsius

        if (compType == STD2::CompBolt) {
            // EN 13445 clause 11.4.3
            allowStress = std::min(Rp02 / 3.0, RmMin / 4.0);

            PR->addDetail("EN13445-3 11 4.3", variableName,
                          "min(Rp02 / 3.0, RmMin / 4.0)",
                          allowStress, "MPa",
                          "min(" + QN(Rp02) + "/3.0; "
                          + QN(RmMin) + "/4.0)", loadCaseNo);
        } else {
            allowStress = std::min(Rp02 / 1.5, RmMin / 2.4);

            PR->addDetail("EN13445-3 6", variableName,
                          "min(Rp02 / 1.5, RmMin / 2.4)",
                          allowStress, "MPa",
                          "min(" + QN(Rp02) + "/1.5; "
                          + QN(RmMin) + "/2.4)", loadCaseNo);
        }
    } else if (matStruct != STD2::MatStructAustenitic
               && matStruct != STD2::MatStructCasting
               && elongPercent < 30.0
               && alternativeRoute) {
        Rp02 = get_Rp02(designTemp, loadCaseNo);
        RmMin = get_RmMin(20, loadCaseNo); // 20 Celsius

        if (compType == STD2::CompBolt) {
            // EN 13445 clause 11.4.3
            allowStress = std::min(Rp02 / 3.0, RmMin / 4.0);

            PR->addDetail("EN13445-3 11 4.3", variableName,
                          "min(Rp02 / 3.0, RmMin / 4.0)",
                          allowStress, "MPa",
                          "min(" + QN(Rp02) + "/3.0; "
                          + QN(RmMin) + "/4.0)", loadCaseNo);
        } else {
            allowStress = std::min(Rp02 / 1.5, RmMin / 1.875);

            PR->addDetail("EN13445-3 6", variableName,
                          "min(Rp02 / 1.5, RmMin / 1.875)",
                          allowStress, "MPa",
                          "min(" + QN(Rp02) + "/1.5; "
                          + QN(RmMin) + "/1.875)", loadCaseNo);
        }
    } else if (matStruct == STD2::MatStructAustenitic
               && 30.0 <= elongPercent
               && elongPercent < 35.0) {
        if (compType == STD2::CompBolt) {
            // EN 13445 clause 11.4.3
            RmMin = get_RmMin(designTemp, loadCaseNo);
            allowStress = RmMin / 4.0;

            PR->addDetail("EN13445-3 11 4.3", variableName,
                          "RmMin / 4.0",
                          allowStress, "MPa",
                          QN(RmMin) + "/4.0)", loadCaseNo);
        } else {
            Rp10 = get_Rp10(designTemp, loadCaseNo);
            allowStress = Rp10 / 1.5;

            PR->addDetail("EN13445-3 6", variableName,
                          "Rp10 / 1.5",
                          allowStress, "MPa",
                          QN(Rp02) + "/1.5)", loadCaseNo);
        }
    } else if (matStruct == STD2::MatStructAustenitic
               && 35.0 <= elongPercent) {
        RmMin = get_RmMin(designTemp, loadCaseNo);

        if (compType == STD2::CompBolt) {
            // EN 13445 clause 11.4.3
            allowStress = RmMin / 4.0;

            PR->addDetail("EN13445-3 11 4.3", variableName,
                          "RmMin / 4.0",
                          allowStress, "MPa",
                          QN(RmMin) + "/4.0)", loadCaseNo);
        } else if (compType == STD2::CompFlange) {
            // // EN 13445 clause 11.5.4.2
            Rp10 = get_Rp10(designTemp);
            allowStress = Rp10 / 1.5;

            PR->addDetail("EN13445-3 11.5.4.2", variableName,
                          "Rp10 / 1.5",
                          allowStress, "MPa",
                          QN(Rp02) + "/1.5)", loadCaseNo);
        } else {
            Rp10 = get_Rp10(designTemp, loadCaseNo);
            allowStress = std::max(Rp10 / 1.5,
                                   std::min(Rp10 / 1.2, RmMin / 3.0));

            PR->addDetail("EN13445-3 6", variableName,
                          "max(Rp10 / 1.5, min(Rp10 / 1.2, RmMin / 3.0))",
                          allowStress, "MPa",
                          "max(" + QN(Rp10) + "/1.5; min(" + QN(Rp10) + "/1.2;"
                          + QN(RmMin) + "/3.0))", loadCaseNo);
        }
    } else if (matStruct == STD2::MatStructCasting) {
        Rp02 = get_Rp02(designTemp, loadCaseNo);
        RmMin = get_RmMin(20, loadCaseNo); // 20 Celsius
        allowStress = std::min(Rp02 / 1.9, RmMin / 3.0);

        PR->addDetail("EN13445-3 6", variableName,
                      "min(Rp02 / 1.9, RmMin / 3.0)",
                      allowStress, "MPa",
                      "min(" + QN(Rp02) + "/1.9; "
                      + QN(RmMin) + "/3.0)", loadCaseNo);
    }

    return allowStress;
}

double STD_EnMaterialHandler::allowableTestStress(
                    double testTemp, STD2::CompType compType,
                    int loadCaseNo, const QString& variableName) {
    STD2::MatStruct matStruct = getMaterialStructure();
    double elongPercent = getElongationPercent();
    bool alternativeRoute = false; // clause 6.3
    double allowStress = 0.0;

    double Rp02 = 0.0;
    double Rp10 = 0.0;
    double RmMin = 0.0;

    if (matStruct != STD2::MatStructAustenitic
            && matStruct != STD2::MatStructCasting
            && elongPercent < 30.0
            && !alternativeRoute) {
        Rp02 = get_Rp02(testTemp, loadCaseNo);
        RmMin = get_RmMin(testTemp, loadCaseNo); // 20 Celsius

        if (compType == STD2::CompBolt) {
            // EN 13445 clause 11.4.3
            allowStress = std::min(Rp02 / 2.0, RmMin / 2.67);

            PR->addDetail("EN13445-3 11 4.3", variableName,
                          "min(Rp02 / 2.0, RmMin / 2.67)",
                          allowStress, "MPa",
                          "min(" + QN(Rp02) + "/2.0; "
                          + QN(RmMin) + "/2.67)", loadCaseNo);
        } else {
            allowStress = Rp02 / 1.05;

            PR->addDetail("EN13445-3 6", variableName,
                          "Rp02 / 1.05",
                          allowStress, "MPa",
                          QN(Rp02) + "/1.05", loadCaseNo);
        }
    } else if (matStruct != STD2::MatStructAustenitic
               && matStruct != STD2::MatStructCasting
               && elongPercent < 30.0
               && alternativeRoute) {
        Rp02 = get_Rp02(testTemp, loadCaseNo);
        RmMin = get_RmMin(testTemp, loadCaseNo); // 20 Celsius

        if (compType == STD2::CompBolt) {
            // EN 13445 clause 11.4.3
            allowStress = std::min(Rp02 / 2.0, RmMin / 2.67);

            PR->addDetail("EN13445-3 11 4.3", variableName,
                          "min(Rp02 / 2.0, RmMin / 2.67)",
                          allowStress, "MPa",
                          "min(" + QN(Rp02) + "/2.0; "
                          + QN(RmMin) + "/2.67)", loadCaseNo);
        } else {
            allowStress = Rp02 / 1.05;

            PR->addDetail("EN13445-3 6", variableName,
                          "Rp02 / 1.05",
                          allowStress, "MPa",
                          QN(Rp02) + "/1.05", loadCaseNo);
        }
    } else if (matStruct == STD2::MatStructAustenitic
               && 30.0 <= elongPercent
               && elongPercent < 35.0) {
        if (compType == STD2::CompBolt) {
            // EN 13445 clause 11.4.3
            RmMin = get_RmMin(testTemp, loadCaseNo);
            allowStress = RmMin / 2.67;

            PR->addDetail("EN13445-3 11 4.3", variableName,
                          "RmMin / 2.67",
                          allowStress, "MPa",
                          QN(RmMin) + "/2.67)", loadCaseNo);
        } else {
            Rp10 = get_Rp10(testTemp, loadCaseNo);
            allowStress = Rp10 / 1.05;

            PR->addDetail("EN13445-3 6", variableName,
                          "Rp10 / 1.05",
                          allowStress, "MPa",
                          QN(Rp02) + "/1.05)", loadCaseNo);
        }
    } else if (matStruct == STD2::MatStructAustenitic
               && 35.0 <= elongPercent) {
        RmMin = get_RmMin(testTemp, loadCaseNo);

        if (compType == STD2::CompBolt) {
            // EN 13445 clause 11.4.3
            allowStress = RmMin / 2.67;

            PR->addDetail("EN13445-3 11 4.3", variableName,
                          "RmMin / 2.67",
                          allowStress, "MPa",
                          QN(RmMin) + "/2.67)", loadCaseNo);
        } else {
            Rp10 = get_Rp10(testTemp, loadCaseNo);
            allowStress = std::max(Rp10 / 1.05, RmMin / 2.0);

            PR->addDetail("EN13445-3 6", variableName,
                          "max(Rp10 / 1.05, RmMin / 2.0))",
                          allowStress, "MPa",
                          "max(" + QN(Rp10) + "/1.05;"
                          + QN(RmMin) + "/2.0)", loadCaseNo);
        }
    } else if (matStruct = STD2::MatStructCasting) {
        Rp02 = get_Rp02(testTemp, loadCaseNo);
        allowStress = Rp02 / 1.33;

        PR->addDetail("EN13445-3 6", variableName,
                      "Rp02 / 1.33",
                      allowStress, "MPa",
                      QN(Rp02) + "/1.33", loadCaseNo);
    }

    return allowStress;
}

double STD_EnMaterialHandler::elasticityModulus(double designTemp,
                                              int loadCaseNo,
                                              const QString& variableName) {
    if (!mCurrentElasModulTable) {
        RB_DEBUG->error("STD_EnMaterialHandler::getElasModul() "
                        "mCurrentElasModulTable NULL ERROR");
        return 0.0;
    }

    double elasModul = getLinInterpValue(
                mCurrentElasModulTable->getContainer("STD_ElasModulList"),
                "temperature", "elasmodul", designTemp);

    PR->addDetail("Material 5", variableName, "EN value at " + QN(designTemp),
                  elasModul, "N/mm2", mCurrentMatName + " table value", loadCaseNo);
    return elasModul;
}

double STD_EnMaterialHandler::thermalExpansion(double designTemp,
                                             int loadCaseNo,
                                             const QString& variableName) {
    if (!mCurrentThermExpTable) {
        RB_DEBUG->error("STD_EnMaterialHandler::getThermExp() "
                        "mCurrentThermExpTable NULL ERROR");
        return 0.0;
    }

    double thermExp = getLinInterpValue(
                mCurrentThermExpTable->getContainer("STD_ThermExpList"),
                "temperature", "thermexp", designTemp);

    PR->addDetail("Material 4", variableName, "EN value at " + QN(designTemp),
                  thermExp, "1/K", mCurrentMatName + " table value", loadCaseNo);
    return thermExp;
}

STD2::MatStruct STD_EnMaterialHandler::getMaterialStructure() {
    if (!mCurrentMaterial) {
        RB_DEBUG->error("STD_EnMaterialHandler::getMaterialStructure() "
                        "mCurrentMaterial NULL ERROR");
        return STD2::MatStructNone;
    }

    int matStruct = mCurrentMaterial->getValue("structure_id").toInt();

    QStringList matStructList; // refer PCALC_EN1591Widget.initModelMapping()
    matStructList << "None" << "Ferritic" << "Austenitic" << "Martensitic"
                  << "Austenitic-Ferritic" << "Casting" << "Non-Ferrous" << "other";
    PR->addDetail("Material 0", "Mat.struct.", matStructList.at(matStruct),
                  matStruct, "-", mCurrentMatName + " table value");
    return (STD2::MatStruct)matStruct;
}

double STD_EnMaterialHandler::getElongationPercent() {
    if (!mCurrentMaterial) {
        RB_DEBUG->error("STD_EnMaterialHandler::getElongationPercent() "
                        "mCurrentMaterial NULL ERROR");
        return 0.0;
    }

    double elongPerc = mCurrentMaterial->getValue("elongafterrupt").toDouble();
    PR->addDetail("Material 0", "A (elong.)", "EN value",
                  elongPerc, "Percent", mCurrentMatName + " table value");
    return elongPerc;
}

/**
 * @brief STD_EnMaterialHandler::get_Rp02 get 0.2% proof strength
 * @param designTemp design temperature [Celsius]
 * @return 0.2% proof strength at temperature [MPa]
 */
double STD_EnMaterialHandler::get_Rp02(double designTemp, int loadCaseNo) {
    if (!mCurrentMaterial) {
        RB_DEBUG->error("STD_EnMaterialHandler::get_Rp02() "
                        "mCurrentMaterial NULL ERROR");
        return 0.0;
    }

    double Rp02 = getLinInterpValue(
                mCurrentMaterial->getContainer("STD_Rp02List"),
                "temperature", "rp02", designTemp);
    PR->addDetail("Material 1", "Rp02", "EN value at " + QN(designTemp),
                  Rp02, "MPa", mCurrentMatName + " table value", loadCaseNo);
    return Rp02;
}

double STD_EnMaterialHandler::get_Rp10(double designTemp, int loadCaseNo) {
    if (!mCurrentMaterial) {
        RB_DEBUG->error("STD_EnMaterialHandler::get_Rp10() "
                        "mCurrentMaterial NULL ERROR");
        return 0.0;
    }

    double Rp10 = getLinInterpValue(
                mCurrentMaterial->getContainer("STD_Rp10List"),
                "temperature", "rp10", designTemp);
    PR->addDetail("Material 1", "Rp10", "EN value at " + QN(designTemp),
                  Rp10, "MPa", mCurrentMatName + " table value", loadCaseNo);
    return Rp10;
}

double STD_EnMaterialHandler::get_RmMin(double designTemp, int loadCaseNo) {
    if (!mCurrentMaterial) {
        RB_DEBUG->error("STD_EnMaterialHandler::get_RmMin() "
                        "mCurrentMaterial NULL ERROR");
        return 0.0;
    }

    double RmMin = getLinInterpValue(
                mCurrentMaterial->getContainer("STD_RmMinList"),
                "temperature", "rmmin", designTemp);
    PR->addDetail("Material 1", "RmMin", "EN value at " + QN(designTemp),
                  RmMin, "MPa", mCurrentMatName + " table value", loadCaseNo);
    return RmMin;
}

bool STD_EnMaterialHandler::loadMaterial(const QString& materialId) {
    QSqlDatabase stdDb = PCALC_MODELFACTORY->getStandardDatabase();
    mCurrentMaterial = mMaterialList->newObject(materialId);
    bool success = mCurrentMaterial->dbRead(stdDb, RB2::ResolveAll);

    // check if reading data of material has been successfull
    RB_ObjectContainer* objRp02List =
            mCurrentMaterial->getContainer("STD_Rp02List");

    if (success && objRp02List && (objRp02List->objectCount() > 0
            /* || ASME allowstress*/)) {
        return true;
    }

    mMaterialList->remove(mCurrentMaterial);
    mCurrentMaterial = nullptr;
    return false;
}

bool STD_EnMaterialHandler::loadElasModulTable(const QString& elasModulTableId) {
    QSqlDatabase stdDb = PCALC_MODELFACTORY->getStandardDatabase();
    mCurrentElasModulTable =  mElasModulTableList->newObject(elasModulTableId);
    bool success = mCurrentElasModulTable->dbRead(stdDb, RB2::ResolveAll);

    RB_ObjectContainer* objElasModulList =
            mCurrentElasModulTable->getContainer("STD_ElasModulList");

    if (success && objElasModulList && objElasModulList->objectCount() > 0) {
        return true;
    }

    mElasModulTableList->remove(mCurrentElasModulTable);
    mCurrentElasModulTable = nullptr;
    return false;
}

bool STD_EnMaterialHandler::loadThemExpTable(const QString& thermExpTableId) {
    QSqlDatabase stdDb = PCALC_MODELFACTORY->getStandardDatabase();
    mCurrentThermExpTable =  mThermExpTableList->newObject(thermExpTableId);
    bool success = mCurrentThermExpTable->dbRead(stdDb, RB2::ResolveAll);

    RB_ObjectContainer* objThermExpList =
            mCurrentThermExpTable->getContainer("STD_ThermExpList");

    if (success && objThermExpList && objThermExpList->objectCount() > 0) {
        return true;
    }

    mThermExpTableList->remove(mCurrentThermExpTable);
    mCurrentThermExpTable = nullptr;
    return false;
}
