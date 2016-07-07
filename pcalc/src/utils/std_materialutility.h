/*****************************************************************
 * $Id: std_materialutility.h 0001 2016-06-17 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_MATERIALUTILITY_H
#define STD_MATERIALUTILITY_H

#include <string>
#include <vector>
#include "rb_utility.h"
#include "std.h"
#include "std_materialhandler.h"

// For unit tests only
//class STD_MaterialUtilityTest;

#define STD_MATERIALUTILITY STD_MaterialUtility::getInstance()

/**
 * @brief Standard material utility providing properties for reports,
 * calculations, etc.
 */
class STD_MaterialUtility : public RB_Utility {

public:
    virtual ~STD_MaterialUtility();
    static STD_MaterialUtility* getInstance();

    bool setCurrentMaterial(const QString& materialIdx);
    bool isValid();
    void refresh() {}

    double allowableDesignStress(double designTemp,
                                 STD2::CompType compType = STD2::CompDefault,
                                 int loadCaseNo = -1,
                                 const QString& variableName = "");
    double allowableTestStress(double testTemp,
                               STD2::CompType compType = STD2::CompDefault,
                               int loadCaseNo = -1,
                               const QString& variableName = "");
    double elasticityModulus(double designTemp,
                             int loadCaseNo = -1,
                             const QString& variableName = "");
    double thermalExpansion(double designTemp,
                            int loadCaseNo = -1,
                            const QString& variableName = "");

    STD2::MatStruct getMaterialStructure();
    double getElongationPercent();

//    // For unit tests only
//    friend class STD_MaterialUtilityTest;

private:
    STD_MaterialUtility();

    static STD_MaterialUtility* mActiveUtility;
    STD_MaterialHandler* mMatHandler;

};

#endif // STD_MATERIALUTILITY_H
