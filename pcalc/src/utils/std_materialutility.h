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
#include "rb_namespace.h"
#include "rb_tablemath.h"
#include "rb_utility.h"
#include "std.h"

// For unit tests only
class STD_MaterialUtilityTest;

#define STD_MATERIALUTILITY STD_MaterialUtility::getInstance()

/**
 * @brief Standard material utility providing properties for reports,
 * calculations, etc.
 */
class STD_MaterialUtility : public Biluna::Calc::RB_TableMath, RB_Utility {

public:
    virtual ~STD_MaterialUtility();
    static STD_MaterialUtility* getInstance();

    bool setCurrentMaterial(const QString& materialIdx);
    bool isValid();
    void refresh() {}

    double allowableDesignStress(double designTemp,
                                 STD2::CompType compType = STD2::CompDefault);
    double allowableTestStress(double testTemp,
                               STD2::CompType compType = STD2::CompDefault);
    double elasticityModulus(double designTemp);
    double thermalExpansion(double designTemp);

    STD2::MatStruct getMaterialStructure();
    double getElongationPercent();

    double get_Rp02(double designTemp);
    double get_Rp10(double designTemp);
    double get_RmMin(double designTemp);

    // For unit tests only
    friend class STD_MaterialUtilityTest;

private:
    STD_MaterialUtility();

    bool loadMaterial(const QString& materialId);
    bool loadElasModulTable(const QString& elasModulTableId);
    bool loadThemExpTable(const QString& thermExpTableId);

    static STD_MaterialUtility* mActiveUtility;
    RB_ObjectContainer* mMaterialList;
    RB_ObjectBase* mCurrentMaterial;
    RB_ObjectContainer* mElasModulTableList;
    RB_ObjectBase* mCurrentElasModulTable;
    RB_ObjectContainer* mThermExpTableList;
    RB_ObjectBase* mCurrentThermExpTable;
    // TODO: External Pressure


};

#endif // STD_MATERIALUTILITY_H
