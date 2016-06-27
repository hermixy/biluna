﻿/*****************************************************************
 * $Id: std_materialutility.cpp 0001 2016-06-17 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_materialutility.h"
#include "pcalc.h"
#include "pcalc_utilityfactory.h"
#include "std_enmaterialhandler.h"

STD_MaterialUtility* STD_MaterialUtility::mActiveUtility = 0;

STD_MaterialUtility::STD_MaterialUtility()
           : /* Biluna::Calc::RB_TableMath(),*/ RB_Utility() {
    mMatHandler = new STD_EnMaterialHandler();

//    RB_DEBUG->print("STD_MaterialUtility::STD_MaterialUtility()");
//    mMaterialList = new RB_ObjectContainer(
//                "", nullptr, "STD_MaterialList", PCALC_OBJECTFACTORY);
//    mCurrentMaterial = nullptr;
//    mElasModulTableList = new RB_ObjectContainer(
//                "", nullptr, "STD_ElasModulTableList", PCALC_OBJECTFACTORY);
//    mCurrentElasModulTable = nullptr;
//    mThermExpTableList = new RB_ObjectContainer(
//                "", nullptr, "STD_ThermExpTableList", PCALC_OBJECTFACTORY);
//    mCurrentThermExpTable = nullptr;
//    // external pressure

    PCALC_UTILITYFACTORY->addUtility(this);
}

STD_MaterialUtility::~STD_MaterialUtility() {
    delete mMatHandler;
    mMatHandler = nullptr;

//    delete mMaterialList;
//    mMaterialList = nullptr;
//    delete mElasModulTableList;
//    mElasModulTableList = nullptr;
//    delete mThermExpTableList;
//    mThermExpTableList = nullptr;
//    // external pressure

    PCALC_UTILITYFACTORY->removeUtility(this);
    mActiveUtility = nullptr;
    RB_DEBUG->print("STD_MaterialUtility::~STD_MaterialUtility()");
}

STD_MaterialUtility* STD_MaterialUtility::getInstance() {
    if (!mActiveUtility) {
        mActiveUtility = new STD_MaterialUtility();
        mActiveUtility->refresh();
    }
    return mActiveUtility;
}

bool STD_MaterialUtility::setCurrentMaterial(const QString& materialIdx) {
    return mMatHandler->setCurrentMaterial(materialIdx);
//    bool success = false;
//    QString materialId = RB2::IdxId(materialIdx);

//    mCurrentMaterial = mMaterialList->getObject(materialId);

//    if (mCurrentMaterial) {
//        success = true;
//    } else {
//        // will also set mCurrentMaterial
//        success = loadMaterial(materialId);

//        if (success) {
//            QString id = mCurrentMaterial->getValue("elasmodultable_id").toString();
//            loadElasModulTable(id);
//            id = mCurrentMaterial->getValue("thermexptable_id").toString();
//            loadThemExpTable(id);
//            // external pressure chart
//        }
//    }

//    return success;
}

/**
 * @brief STD_MaterialUtility::isValid
 * @return true if valid material object
 */
bool STD_MaterialUtility::isValid() {
    return mMatHandler->isValid();
//    return mCurrentMaterial != nullptr;
}


double STD_MaterialUtility::allowableDesignStress(double designTemp,
                                                  STD2::CompType compType,
                                                  int loadCaseNo,
                                                  const QString& variableName) {
    double result = mMatHandler->allowableDesignStress(
                designTemp, compType, loadCaseNo, variableName);
    return result;
}

double STD_MaterialUtility::allowableTestStress(double testTemp,
                                                STD2::CompType compType,
                                                int loadCaseNo,
                                                const QString& variableName) {
    double result = mMatHandler->allowableTestStress(
                testTemp, compType, loadCaseNo, variableName);
    return result;
}

double STD_MaterialUtility::elasticityModulus(double designTemp,
                                              int loadCaseNo,
                                              const QString& variableName) {
    return mMatHandler->elasticityModulus(designTemp, loadCaseNo, variableName);
//    if (!mCurrentElasModulTable) {
//        RB_DEBUG->error("STD_MaterialUtility::getElasModul() "
//                        "mCurrentElasModulTable NULL ERROR");
//        return 0.0;
//    }

//    double elasModul = getLinInterpValue(
//                mCurrentElasModulTable->getContainer("STD_ElasModulList"),
//                "temperature", "elasmodul", designTemp);
////    PR->addDetail("Material 0", variableName, "EN13555 value",
////                  elasModul, "N/mm2", "Table value", loadCaseNo);
//    return elasModul;
}

double STD_MaterialUtility::thermalExpansion(double designTemp,
                                             int loadCaseNo,
                                             const QString &variableName) {
    return mMatHandler->thermalExpansion(designTemp, loadCaseNo, variableName);
//    if (!mCurrentThermExpTable) {
//        RB_DEBUG->error("STD_MaterialUtility::getThermExp() "
//                        "mCurrentThermExpTable NULL ERROR");
//        return 0.0;
//    }

//    double thermExp = getLinInterpValue(
//                mCurrentThermExpTable->getContainer("STD_ThermExpList"),
//                "temperature", "thermexp", designTemp);
//    return thermExp;
}

STD2::MatStruct STD_MaterialUtility::getMaterialStructure() {
    return mMatHandler->getMaterialStructure();
//    if (!mCurrentMaterial) {
//        RB_DEBUG->error("STD_MaterialUtility::getMaterialStructure() "
//                        "mCurrentMaterial NULL ERROR");
//        return STD2::MatStructNone;
//    }

//    int matStruct = mCurrentMaterial->getValue("structure_id").toInt();
//    return (STD2::MatStruct)matStruct;
}

double STD_MaterialUtility::getElongationPercent() {
    return mMatHandler->getElongationPercent();
//    if (!mCurrentMaterial) {
//        RB_DEBUG->error("STD_MaterialUtility::getElongationPercent() "
//                        "mCurrentMaterial NULL ERROR");
//        return 0.0;
//    }

//    double elongPerc = mCurrentMaterial->getValue("elongafterrupt").toDouble();
//    return elongPerc;
}

/**
 * @brief STD_MaterialUtility::get_Rp02 get 0.2% proof strength
 * @param designTemp design temperature [Celsius]
 * @return 0.2% proof strength at temperature [MPa]
 */
//double STD_MaterialUtility::get_Rp02(double designTemp) {
//    if (!mCurrentMaterial) {
//        RB_DEBUG->error("STD_MaterialUtility::get_Rp02() "
//                        "mCurrentMaterial NULL ERROR");
//        return 0.0;
//    }

//    double Rp02 = getLinInterpValue(
//                mCurrentMaterial->getContainer("STD_Rp02List"),
//                "temperature", "rp02", designTemp);
//    return Rp02;
//}

//double STD_MaterialUtility::get_Rp10(double designTemp) {
//    if (!mCurrentMaterial) {
//        RB_DEBUG->error("STD_MaterialUtility::get_Rp10() "
//                        "mCurrentMaterial NULL ERROR");
//        return 0.0;
//    }

//    double Rp10 = getLinInterpValue(
//                mCurrentMaterial->getContainer("STD_Rp10List"),
//                "temperature", "rp10", designTemp);
//    return Rp10;
//}

//double STD_MaterialUtility::get_RmMin(double designTemp) {
//    if (!mCurrentMaterial) {
//        RB_DEBUG->error("STD_MaterialUtility::get_RmMin() "
//                        "mCurrentMaterial NULL ERROR");
//        return 0.0;
//    }

//    double RmMin = getLinInterpValue(
//                mCurrentMaterial->getContainer("STD_RmMinList"),
//                "temperature", "rmmin", designTemp);
//    return RmMin;
//}

//bool STD_MaterialUtility::loadMaterial(const QString& materialId) {
//    QSqlDatabase stdDb = PCALC_MODELFACTORY->getStandardDatabase();
//    mCurrentMaterial = mMaterialList->newObject(materialId);
//    bool success = mCurrentMaterial->dbRead(stdDb, RB2::ResolveAll);

//    // check if reading data of material has been successfull
//    RB_ObjectContainer* objRp02List =
//            mCurrentMaterial->getContainer("STD_Rp02List");

//    if (success && objRp02List && (objRp02List->objectCount() > 0
//            /* || ASME allowstress*/)) {
//        return true;
//    }

//    mMaterialList->remove(mCurrentMaterial);
//    mCurrentMaterial = nullptr;
//    return false;
//}

//bool STD_MaterialUtility::loadElasModulTable(const QString& elasModulTableId) {
//    QSqlDatabase stdDb = PCALC_MODELFACTORY->getStandardDatabase();
//    mCurrentElasModulTable =  mElasModulTableList->newObject(elasModulTableId);
//    bool success = mCurrentElasModulTable->dbRead(stdDb, RB2::ResolveAll);

//    RB_ObjectContainer* objElasModulList =
//            mCurrentElasModulTable->getContainer("STD_ElasModulList");

//    if (success && objElasModulList && objElasModulList->objectCount() > 0) {
//        return true;
//    }

//    mElasModulTableList->remove(mCurrentElasModulTable);
//    mCurrentElasModulTable = nullptr;
//    return false;
//}

//bool STD_MaterialUtility::loadThemExpTable(const QString& thermExpTableId) {
//    QSqlDatabase stdDb = PCALC_MODELFACTORY->getStandardDatabase();
//    mCurrentThermExpTable =  mThermExpTableList->newObject(thermExpTableId);
//    bool success = mCurrentThermExpTable->dbRead(stdDb, RB2::ResolveAll);

//    RB_ObjectContainer* objThermExpList =
//            mCurrentThermExpTable->getContainer("STD_ThermExpList");

//    if (success && objThermExpList && objThermExpList->objectCount() > 0) {
//        return true;
//    }

//    mThermExpTableList->remove(mCurrentThermExpTable);
//    mCurrentThermExpTable = nullptr;
//    return false;
//}