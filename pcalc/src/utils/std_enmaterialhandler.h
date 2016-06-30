/*****************************************************************
 * $Id: std_enmaterialhandler.h 0001 2016-06-24 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_ENMATERIALHANDLER_H
#define STD_ENMATERIALHANDLER_H

#include <string>
#include <vector>
#include "rb_namespace.h"
#include "rb_tablemath.h"
#include "std.h"
#include "std_materialhandler.h"

// For unit tests only
class STD_EnMaterialHandlerTest;

/**
 * @brief Material handler providing EN properties for reports,
 * calculations, etc.
 */
class STD_EnMaterialHandler : public STD_MaterialHandler,
                            Biluna::Calc::RB_TableMath {

public:
    STD_EnMaterialHandler();
    virtual ~STD_EnMaterialHandler();

    virtual bool setCurrentMaterial(const QString& materialIdx);
    virtual bool isValid();
    virtual void refresh();

    virtual double allowableDesignStress(double designTemp,
                                 STD2::CompType compType = STD2::CompDefault,
                                 int loadCaseNo = -1,
                                 const QString& variableName = "");
    virtual double allowableTestStress(double testTemp,
                               STD2::CompType compType = STD2::CompDefault,
                               int loadCaseNo = -1,
                               const QString& variableName = "");
    virtual double elasticityModulus(double designTemp,
                             int loadCaseNo = -1,
                             const QString& variableName = "");
    virtual double thermalExpansion(double designTemp,
                            int loadCaseNo = -1,
                            const QString& variableName = "");

    virtual STD2::MatStruct getMaterialStructure();
    virtual double getElongationPercent();

    // For unit tests only
    friend class STD_EnMaterialHandlerTest;

protected:
    virtual double allowableDesignStressBolt(double designTemp,
                                 int loadCaseNo = -1,
                                 const QString& variableName = "");
    virtual double allowableTestStressBolt(double testTemp,
                               int loadCaseNo = -1,
                               const QString& variableName = "");

    double get_Rp02(double designTemp, int loadCaseNo = -1);
    double get_Rp10(double designTemp, int loadCaseNo = -1);
    double get_RmMin(double designTemp, int loadCaseNo = -1);

private:
    bool loadMaterial(const QString& materialId);
    bool loadElasModulTable(const QString& elasModulTableId);
    bool loadThemExpTable(const QString& thermExpTableId);

    RB_ObjectContainer* mMaterialList;
    RB_ObjectBase* mCurrentMaterial;
    QString mCurrentMatName;
    RB_ObjectContainer* mElasModulTableList;
    RB_ObjectBase* mCurrentElasModulTable;
    RB_ObjectContainer* mThermExpTableList;
    RB_ObjectBase* mCurrentThermExpTable;
    // TODO: External Pressure

};

#endif // STD_ENMATERIALHANDLER_H
