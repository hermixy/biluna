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

bool STD_MaterialUtility::loadMaterial(const QString& materialId) {
    QSqlDatabase stdDb = PCALC_MODELFACTORY->getStandardDatabase();
    mCurrentMaterial = mMaterialList->newObject(materialId);
    bool success = mCurrentMaterial->dbRead(stdDb, RB2::ResolveAll);

    // check if reading data of material has been successfull
    RB_ObjectContainer* objC =
            mCurrentMaterial->getContainer("STD_EN13555QsminLList");

    if (success && objC && objC->objectCount() > 0) {
        return true;
    }

    mMaterialList->remove(mCurrentMaterial);
    mCurrentMaterial = nullptr;
    return false;
}
