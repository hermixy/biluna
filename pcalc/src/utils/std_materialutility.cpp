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
#include "pcalc_utilityfactory.h"
#include "std_enmaterialhandler.h"

STD_MaterialUtility* STD_MaterialUtility::mActiveUtility = 0;

STD_MaterialUtility::STD_MaterialUtility() : RB_Utility() {
    RB_DEBUG->print("STD_MaterialUtility::STD_MaterialUtility()");
    mMatHandler = new STD_EnMaterialHandler();
    PCALC_UTILITYFACTORY->addUtility(this);
}

STD_MaterialUtility::~STD_MaterialUtility() {
    delete mMatHandler;
    mMatHandler = nullptr;
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
}

/**
 * @brief STD_MaterialUtility::isValid
 * @return true if valid material object
 */
bool STD_MaterialUtility::isValid() {
    return mMatHandler->isValid();
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
}

double STD_MaterialUtility::thermalExpansion(double designTemp,
                                             int loadCaseNo,
                                             const QString &variableName) {
    return mMatHandler->thermalExpansion(designTemp, loadCaseNo, variableName);
}

STD2::MatStruct STD_MaterialUtility::getMaterialStructure() {
    return mMatHandler->getMaterialStructure();
}

double STD_MaterialUtility::getElongationPercent() {
    return mMatHandler->getElongationPercent();
}
