/*****************************************************************
 * $Id: std_materialutility.cpp 0001 2016-06-17 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_materialutility.h"
#include "pcalc.h"
#include "pcalc_modelfactory.h"
#include "pcalc_objectfactory.h"
#include "pcalc_utilityfactory.h"

STD_MaterialUtility* STD_MaterialUtility::mActiveUtility = 0;

STD_MaterialUtility::STD_MaterialUtility()
            : Biluna::Calc::RB_TableMath(), RB_Utility() {
    RB_DEBUG->print("STD_MaterialUtility::STD_MaterialUtility()");
    mMaterialList = new RB_ObjectContainer("", nullptr,
                                         "STD_MaterialList",
                                         PCALC_OBJECTFACTORY);
    mCurrentMaterial = nullptr;
    PCALC_UTILITYFACTORY->addUtility(this);
}

STD_MaterialUtility::~STD_MaterialUtility() {
    delete mMaterialList;
    mMaterialList = nullptr;

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
    bool success = false;
    QString materialId = RB2::IdxId(materialIdx);

    mCurrentMaterial = mMaterialList->getObject(materialId);

    if (mCurrentMaterial) {
        success = true;
    } else {
        // will also set mCurrentMaterial
        success = loadMaterial(materialId);
    }

    return success;
}

/**
 * @brief STD_MaterialUtility::isValid
 * @return true if valid material object
 */
bool STD_MaterialUtility::isValid() {
    return mCurrentMaterial != nullptr;
}


double STD_MaterialUtility::allowableDesignStress(double designTemp,
                                                  STD2::CompType compType) {
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
        Rp02 = get_Rp02(designTemp);
        RmMin = get_RmMin(20); // 20 Celsius

        if (compType == STD2::CompBolt) {
            allowStress = std::min(Rp02 / 3.0, RmMin / 4.0);
        } else {
            allowStress = std::min(Rp02 / 1.5, RmMin / 2.4);
        }
    } else if (matStruct != STD2::MatStructAustenitic
               && matStruct != STD2::MatStructCasting
               && elongPercent < 30.0
               && alternativeRoute) {
        Rp02 = get_Rp02(designTemp);
        RmMin = get_RmMin(20); // 20 Celsius

        if (compType == STD2::CompBolt) {
            allowStress = std::min(Rp02 / 3.0, RmMin / 4.0);
        } else {
            allowStress = std::min(Rp02 / 1.5, RmMin / 1.875);
        }
    } else if (matStruct == STD2::MatStructAustenitic
               && 30.0 <= elongPercent
               && elongPercent < 35.0) {
        if (compType == STD2::CompBolt) {
            RmMin = get_RmMin(designTemp);
            allowStress = RmMin / 4.0;
        } else {
            Rp10 = get_Rp10(designTemp);
            allowStress = Rp10 / 1.5;
        }
    } else if (matStruct == STD2::MatStructAustenitic
               && 35.0 <= elongPercent) {
        RmMin = get_RmMin(designTemp);

        if (compType == STD2::CompBolt) {
            allowStress = RmMin / 4.0;
        } else if (compType == STD2::CompFlange) {
            Rp10 = get_Rp10(designTemp);
            allowStress = Rp10 / 1.5;
        } else {
            Rp10 = get_Rp10(designTemp);
            allowStress = std::max(Rp10 / 1.5,
                                   std::min(Rp10 / 1.2, RmMin / 3.0));
        }
    } else if (matStruct == STD2::MatStructCasting) {
        Rp02 = get_Rp02(designTemp);
        RmMin = get_RmMin(20); // 20 Celsius
        allowStress = std::min(Rp02 / 1.9, RmMin / 3.0);
    }

    return allowStress;
}

double STD_MaterialUtility::allowableTestStress(double testTemp,
                                                STD2::CompType compType) {
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
        Rp02 = get_Rp02(testTemp);
        RmMin = get_RmMin(testTemp); // 20 Celsius

        if (compType == STD2::CompBolt) {
            allowStress = std::min(Rp02 / 2.0, RmMin / 2.67);
        } else {
            allowStress = Rp02 / 1.05;
        }
    } else if (matStruct != STD2::MatStructAustenitic
               && matStruct != STD2::MatStructCasting
               && elongPercent < 30.0
               && alternativeRoute) {
        Rp02 = get_Rp02(testTemp);
        RmMin = get_RmMin(testTemp); // 20 Celsius

        if (compType == STD2::CompBolt) {
            allowStress = std::min(Rp02 / 2.0, RmMin / 2.67);
        } else {
            allowStress = Rp02 / 1.05;
        }
    } else if (matStruct == STD2::MatStructAustenitic
               && 30.0 <= elongPercent
               && elongPercent < 35.0) {
        if (compType == STD2::CompBolt) {
            RmMin = get_RmMin(testTemp);
            allowStress = RmMin / 2.67;
        } else {
            Rp10 = get_Rp10(testTemp);
            allowStress = Rp10 / 1.05;
        }
    } else if (matStruct == STD2::MatStructAustenitic
               && 35.0 <= elongPercent) {
        RmMin = get_RmMin(testTemp);

        if (compType == STD2::CompBolt) {
            allowStress = RmMin / 2.67;
        } else {
            Rp10 = get_Rp10(testTemp);
            allowStress = std::max(Rp10 / 1.05, RmMin / 2.0);
        }
    } else if (matStruct = STD2::MatStructCasting) {
        Rp02 = get_Rp02(testTemp);
        allowStress = Rp02 / 1.33;
    }

    return allowStress;
}

STD2::MatStruct STD_MaterialUtility::getMaterialStructure() {
    if (!mCurrentMaterial) {
        RB_DEBUG->error("STD_MaterialUtility::getMaterialStructure() "
                        "mCurrentMaterial NULL ERROR");
        return STD2::MatStructNone;
    }

    int matStruct = mCurrentMaterial->getValue("structure_id").toInt();
    return (STD2::MatStruct)matStruct;
}

double STD_MaterialUtility::getElongationPercent() {
    if (!mCurrentMaterial) {
        RB_DEBUG->error("STD_MaterialUtility::getElongationPercent() "
                        "mCurrentMaterial NULL ERROR");
        return 0.0;
    }

    double elongPerc = mCurrentMaterial->getValue("elongafterrupt").toDouble();
    return elongPerc;
}

/**
 * @brief STD_MaterialUtility::get_Rp02 get 0.2% proof strength
 * @param designTemp design temperature [Celsius]
 * @return 0.2% proof strength at temperature [MPa]
 */
double STD_MaterialUtility::get_Rp02(double designTemp) {
    if (!mCurrentMaterial) {
        RB_DEBUG->error("STD_MaterialUtility::get_Rp02() "
                        "mCurrentMaterial NULL ERROR");
        return 0.0;
    }

    double Rp02 = getLinInterpValue(
                mCurrentMaterial->getContainer("STD_Rp02List"),
                "temperature", "rp02", designTemp);
    return Rp02;
}

double STD_MaterialUtility::get_Rp10(double designTemp) {
    if (!mCurrentMaterial) {
        RB_DEBUG->error("STD_MaterialUtility::get_Rp10() "
                        "mCurrentMaterial NULL ERROR");
        return 0.0;
    }

    double Rp10 = getLinInterpValue(
                mCurrentMaterial->getContainer("STD_Rp10List"),
                "temperature", "rp10", designTemp);
    return Rp10;
}

double STD_MaterialUtility::get_RmMin(double designTemp) {
    if (!mCurrentMaterial) {
        RB_DEBUG->error("STD_MaterialUtility::get_RmMin() "
                        "mCurrentMaterial NULL ERROR");
        return 0.0;
    }

    double RmMin = getLinInterpValue(
                mCurrentMaterial->getContainer("STD_RmMinList"),
                "temperature", "rmmin", designTemp);
    return RmMin;
}

bool STD_MaterialUtility::loadMaterial(const QString& materialId) {
    QSqlDatabase stdDb = PCALC_MODELFACTORY->getStandardDatabase();
    mCurrentMaterial = mMaterialList->newObject(materialId);
    bool success = mCurrentMaterial->dbRead(stdDb, RB2::ResolveAll);

    // check if reading data of material has been successfull
    RB_ObjectContainer* objC =
            mCurrentMaterial->getContainer("STD_Rp02List");

    if (success && objC && objC->objectCount() > 0) {
        return true;
    }

    mMaterialList->remove(mCurrentMaterial);
    mCurrentMaterial = nullptr;
    return false;
}
